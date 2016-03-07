#ifndef FSP_HPP
#define FSP_HPP

#include "graph.hpp"

#include <set>
#include <utility>

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
    if (boost::get (boost::edge_type, g, e) == EDGE_T::UP)
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
    if (boost::get (boost::edge_type, g, e) == EDGE_T::DOWN)
      {
        // The downstream vertex.
        Vertex <G> t = boost::target (e, g);
        s.insert (evp <G> (e, t));
      }

  return s;
}

template<typename G>
void
fsp_term (const G &g, Vertex <G> cn, Vertex <G> pn, Path <G> p, v2lp <G> &r)
{
  r[cn].push_back(p);
}

template <typename G>
void
fsp_arn (const G &g, Vertex <G> cn, Vertex <G> pn, Path <G> p, v2lp <G> &r)
{
  assert (boost::get (boost::vertex_type, g, cn) == VERTEX_T::ARN);

  for(const auto &e: make_iterator_range (out_edges (cn, g)))
    {
      // The downstream vertex.
      Vertex <G> t = boost::target (e, g);
      fsp (g, t, cn, p, r);
    }
}

template <typename G>
void
fsp_prn (const G &g, Vertex <G> cn, Vertex <G> pn, Path <G> p, v2lp <G> &r)
{
  assert (boost::get (boost::vertex_type, g, cn) == VERTEX_T::PRN);

  evp <G> up = get_upstream (g, cn);

  if (up.second == pn)
    {
      // The first case (see above), we are going downstream.
    }
  else
    {
      // The second case (see above), we are climbing upstream.
    }
}

// Find shortest paths in a PON.
template <typename G>
void
fsp (const G &g, Vertex <G> cn, Vertex <G> pn, Path <G> p, v2lp <G> &r)
{
  VERTEX_T t = boost::get (boost::vertex_type, g, cn);

  switch (t)
    {
    case VERTEX_T::ONU:
    case VERTEX_T::ICO:
    case VERTEX_T::OLT:
      fsp_term (g, cn, pn, p, r);
      break;
      
    case VERTEX_T::PRN:
      fsp_prn (g, cn, pn, p, r);
      break;

    case VERTEX_T::ARN:
      fsp_arn (g, cn, pn, p, r);
      break;

    default:
      abort();
    }
}

// Find shortest paths in a PON from the given cn to all ONUs and
// ICOs.
template <typename G>
void
fsp (const G &g, Vertex <G> cn, v2lp <G> &r)
{
  fsp (g, cn, G::null_vertex(), Path <G>(), r);
}

#endif /* FSP_HPP */
