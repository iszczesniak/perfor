#ifndef FSP_HPP
#define FSP_HPP

#include "graph.hpp"
#include "utils.hpp"

#include <boost/graph/filtered_graph.hpp>
#include <boost/range.hpp>

#include <algorithm>
#include <set>
#include <utility>

template <typename G>
class fsp
{
  // The graph we operate on.
  G &m_g;

  // The set of visited vertexes.
  Vset <G> vv;

  // The path we're building.
  Path <G> p;
  
public:
  // Find shortest paths in a PON from the given cn to all ONUs and
  // ICOs.
  fsp (G &g, Vertex <G> cn): m_g (g)
  {
    // We don't want to examine the null vertex, which we can
    // encounter when a node was filtered out.
    vv.insert (G::null_vertex ());
    fsp_generic (cn, G::null_vertex ());
  }

private:
  // Return the upstream evp for the given node.
  Vertex <G>
  get_upstream (Vertex <G> v)
  {
    for(const auto &e: make_iterator_range (out_edges (v, m_g)))
      if (boost::get (boost::edge_type, m_g, e) == DIR_T::UP)
        // The upstream vertex.
        return boost::target (e, m_g);

    // It can happen that a node doesn't have an upstream node,
    // because it was filtered out.
    return G::null_vertex ();
  }

  // Return the set of downstream evps for the given node.
  Vset <G>
  get_downstream (Vertex <G> v)
  {
    Vset <G> s;

    for(const auto &e: make_iterator_range (out_edges (v, m_g)))
      if (boost::get (boost::edge_type, m_g, e) == DIR_T::DOWN)
        // The downstream vertex.
        s.insert (boost::target (e, m_g));

    return s;
  }

  // We can get to an ONU only downstream.  We don't start the search
  // from here.  Here we only record the path, because it's the end of
  // it.
  void
  fsp_onu (Vertex <G> cn, Vertex <G> pn)
  {
    assert (boost::get (boost::vertex_type, m_g, cn) == VERTEX_T::ONU);
    assert (pn != G::null_vertex ());
    boost::get (boost::vertex_paths, m_g, cn).push_back (p);
  }

  // If we start the search at the ICO, we climb upstream.  If we get
  // here downstream, we record the path.
  void
  fsp_ico (Vertex <G> cn, Vertex <G> pn)
  {
    assert (boost::get (boost::vertex_type, m_g, cn) == VERTEX_T::ICO);

    if (pn == G::null_vertex ())
      fsp_generic (get_upstream (cn), cn);
    else
      boost::get (boost::vertex_paths, m_g, cn).push_back (p);
  }

  // For active nodes: OLT or ARN.  Here fan out the search.
  void
  fsp_an (Vertex <G> cn, Vertex <G> pn)
  {
    VERTEX_T t = boost::get (boost::vertex_type, m_g, cn);
    assert (t == VERTEX_T::ARN || t == VERTEX_T::OLT);

    for(const auto &e: make_iterator_range (out_edges (cn, m_g)))
      {
        // The downstream vertex.
        Vertex <G> t = boost::target (e, m_g);
        fsp_generic (t, cn);
      }
  }

  // Passive node: PRN.
  void
  fsp_pn (Vertex <G> cn, Vertex <G> pn)
  {
    assert (boost::get (boost::vertex_type, m_g, cn) == VERTEX_T::PRN);

    // The upstream node.
    Vertex <G> up = get_upstream (cn);

    if (up == pn)
      // We're are going downstream.
      for(auto const dn: get_downstream (cn))
        fsp_generic (dn, cn);
    else
      // We are climbing upstream.
      fsp_generic (up, cn);
  }

  // Find shortest paths in a PON.
  void
  fsp_generic (Vertex <G> cn, Vertex <G> pn)
  {
    // Examine cn only if it's not already in the path.
    if (vv.find (cn) == vv.end())
      {
        // Remember the vertex is part of the path.
        vv.insert(cn);

        VERTEX_T t = boost::get (boost::vertex_type, m_g, cn);

        if (pn != G::null_vertex())
          {
            Edge <G> e;
            bool s;
            std::tie (e, s) = boost::edge (pn, cn, m_g);
            assert (s);
            p.push_back (e);
          }

        switch (t)
          {
          case VERTEX_T::ONU:
            fsp_onu (cn, pn);
            break;

          case VERTEX_T::ICO:
            fsp_ico (cn, pn);
            break;
      
          case VERTEX_T::PRN:
            fsp_pn (cn, pn);
            break;

          case VERTEX_T::ARN:
          case VERTEX_T::OLT:
            fsp_an (cn, pn);
            break;

          default:
            abort();
          }

        if (pn != G::null_vertex())
          p.pop_back ();

        vv.erase (cn);
      }
  }
};

// Compare containers according to their sizes.
template <typename C>
struct path_cmp
{
  bool operator () (const C &a, const C &b)
  {
    return a.size() < b.size();
  }
};

template <typename G>
void
sort_paths (G &g)
{
  // Iterate over every vertex and sort its paths.
  for(const auto &v: make_iterator_range(boost::vertices(g)))
    {
      auto &l = boost::get(boost::vertex_paths, g, v);
      std::sort(l.begin(), l.end(), path_cmp <Path <G> > ());
    }
}

// This function fills in shortest paths in a graph.
template <typename G>
void
fill_paths (G &g)
{
  // The type of the filtered graph.
  typedef typename boost::vertex_subset_complement_filter<G, Vset <G> >::type
  FilteredG;

  // The set of excluded vertexes.
  Vset <G> ev;
  // The vertex predicate.
  boost::is_not_in_subset <Vset <G> > vp (ev);
  // The filtered graph.
  FilteredG fg (g, boost::keep_all (), vp);

  // Iterate over OLT and ICOs, and find paths to all other nodes.
  for(auto s: get_nodes (g, VERTEX_T::OLT, VERTEX_T::ICO))
    {
      fsp <FilteredG> (fg, s);
      ev.insert (s);
    }

  sort_paths (g);
}

#endif /* FSP_HPP */
