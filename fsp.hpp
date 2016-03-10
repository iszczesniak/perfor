#ifndef FSP_HPP
#define FSP_HPP

#include "graph.hpp"
#include "utils.hpp"

#include <set>
#include <utility>

#include <iostream>

template <typename G>
class fsp
{
  // The pair of an edge and a vertex.
  typedef std::pair <Edge <G>, Vertex <G> > evp;

  // The set of the evp.
  typedef std::set <evp> evpset;

  // The graph we operate on.
  const G &m_g;

  // Here we store the results;
  v2lp <G> &m_r;

  // The set of visited vertexes.
  Vset <G> vv;

  // The path we're building.
  Path <G> p;
  
public:
  // Find shortest paths in a PON from the given cn to all ONUs and
  // ICOs.
  fsp (const G &g, Vertex <G> cn, v2lp <G> &r): m_g (g), m_r (r)
  {
    fsp_generic (cn, G::null_vertex());
  }

private:
  // Return the upstream evp for the given node.
  evp
  get_upstream (Vertex <G> v)
  {
    for(const auto &e: make_iterator_range (out_edges (v, m_g)))
      if (boost::get (boost::edge_type, m_g, e) == DIR_T::UP)
        {
          // The upstream vertex.
          Vertex <G> t = boost::target (e, m_g);
          return std::make_pair (e, t);
        }

    abort();
  }

  // Return the set of downstream evps for the given node.
  evpset
  get_downstream (Vertex <G> v)
  {
    evpset s;

    for(const auto &e: make_iterator_range (out_edges (v, m_g)))
      if (boost::get (boost::edge_type, m_g, e) == DIR_T::DOWN)
        {
          // The downstream vertex.
          Vertex <G> t = boost::target (e, m_g);
          s.insert (evp (e, t));
        }

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
    m_r[cn].push_back (p);
  }

  // If we start the search at the ICO, we climb upstream.  If we get
  // here downstream, we record the path.
  void
  fsp_ico (Vertex <G> cn, Vertex <G> pn)
  {
    assert (boost::get (boost::vertex_type, m_g, cn) == VERTEX_T::ICO);

    if (pn == G::null_vertex ())
      {
        evp e = get_upstream (cn);
        fsp_generic (e.second, cn);
      }
    else
      m_r[cn].push_back (p);
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
    Vertex <G> up = get_upstream (cn).second;

    if (up == pn)
      // We're are going downstream.
      for(auto const e: get_downstream (cn))
        fsp_generic (e.second, cn);
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

        std::cout << "p.size = " << p.size() << std::endl;
        print_path (m_g, p, std::cout);
        std::cout << "----------------------------------------" << std::endl;

        switch (t)
          {
          case VERTEX_T::ONU:
          case VERTEX_T::ICO:
            fsp_onu (cn, pn);
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

#endif /* FSP_HPP */
