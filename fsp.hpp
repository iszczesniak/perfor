#ifndef FSP_HPP
#define FSP_HPP

#include "graph.hpp"
#include "utils.hpp"

#include <set>
#include <utility>

#include <iostream>

// The pair of an edge and a vertex.
template <typename G>
using evp = std::pair <Edge <G>, Vertex <G> >;

// The set of the evp.
template <typename G>
using evpset = std::set <evp <G> >;

// Return the upstream evp for the given node.
template <typename G>
evp <G>
get_upstream (const G &g, Vertex <G> v)
{
  for(const auto &e: make_iterator_range (out_edges (v, g)))
    if (boost::get (boost::edge_type, g, e) == DIR_T::UP)
      {
        // The upstream vertex.
        Vertex <G> t = boost::target (e, g);
        return std::make_pair (e, t);
      }

  abort();
}

// Return the set of downstream evps for the given node.
template <typename G>
evpset <G>
get_downstream (const G &g, Vertex <G> v)
{
  evpset <G> s;

  for(const auto &e: make_iterator_range (out_edges (v, g)))
    if (boost::get (boost::edge_type, g, e) == DIR_T::DOWN)
      {
        // The downstream vertex.
        Vertex <G> t = boost::target (e, g);
        s.insert (evp <G> (e, t));
      }

  return s;
}

// We can get to an ONU only downstream.  We don't start the search
// from here.  Here we only record the path, because it's the end of
// it.
template<typename G>
void
fsp_onu (const G &g, Vertex <G> cn, Vertex <G> pn, Path <G> &p, v2lp <G> &r)
{
  assert (boost::get (boost::vertex_type, g, cn) == VERTEX_T::ONU);
  assert (pn != G::null_vertex ());
  r[cn].push_back (p);
}

// If we start the search at the ICO, we climb upstream.  If we get
// here downstream, we record the path.
template<typename G>
void
fsp_ico (const G &g, Vertex <G> cn, Vertex <G> pn, Path <G> &p, v2lp <G> &r)
{
  assert (boost::get (boost::vertex_type, g, cn) == VERTEX_T::ICO);

  if (pn == G::null_vertex ())
    {
      evp <G> e = get_upstream (g, cn);
      fsp (g, e.second, cn, p, r);
    }
  else
    r[cn].push_back (p);
}

// For active nodes: OLT or ARN.  Here fan out the search.
template <typename G>
void
fsp_an (const G &g, Vertex <G> cn, Vertex <G> pn, Path <G> &p, v2lp <G> &r)
{
  VERTEX_T t = boost::get (boost::vertex_type, g, cn);
  assert (t == VERTEX_T::ARN || t == VERTEX_T::OLT);

  for(const auto &e: make_iterator_range (out_edges (cn, g)))
    {
      // The downstream vertex.
      Vertex <G> t = boost::target (e, g);
      fsp (g, t, cn, p, r);
    }
}

// Passive node: PRN.
template <typename G>
void
fsp_pn (const G &g, Vertex <G> cn, Vertex <G> pn, Path <G> &p, v2lp <G> &r)
{
  assert (boost::get (boost::vertex_type, g, cn) == VERTEX_T::PRN);

  // The upstream node.
  Vertex <G> up = get_upstream (g, cn).second;

  if (up == pn)
    // We're are going downstream.
    for(auto const evp: get_downstream (g, cn))
      fsp (g, evp.second, cn, p, r);
  else
    // We are climbing upstream.
    fsp (g, up, cn, p, r);
}

// Find shortest paths in a PON.
template <typename G>
void
fsp (const G &g, Vertex <G> cn, Vertex <G> pn, Path <G> &p, v2lp <G> &r)
{
  VERTEX_T t = boost::get (boost::vertex_type, g, cn);

  if (pn != G::null_vertex())
    {
      Edge <G> e;
      bool s;
      std::tie (e, s) = boost::edge (pn, cn, g);
      assert (s);
      p.push_back (e);
    }

  std::cout << "p.size = " << p.size() << std::endl;
  print_path (g, p, std::cout);
  std::cout << "----------------------------------------" << std::endl;

  switch (t)
    {
    case VERTEX_T::ONU:
    case VERTEX_T::ICO:
      fsp_onu (g, cn, pn, p, r);
      break;
      
    case VERTEX_T::PRN:
      fsp_pn (g, cn, pn, p, r);
      break;

    case VERTEX_T::ARN:
    case VERTEX_T::OLT:
      fsp_an (g, cn, pn, p, r);
      break;

    default:
      abort();
    }

  if (pn != G::null_vertex())
    p.pop_back ();
}

// Find shortest paths in a PON from the given cn to all ONUs and
// ICOs.
template <typename G>
void
fsp (const G &g, Vertex <G> cn, v2lp <G> &r)
{
  Path <G> p;
  fsp (g, cn, G::null_vertex(), p, r);
}

#endif /* FSP_HPP */
