#ifndef BFS_HPP
#define BFS_HPP

#include "args.hpp"
#include "graph.hpp"

#include <map>
#include <random>
#include <sstream>

// The pair of an edge and a vertex.
template <typename G>
using evp = std::pair <Edge <G>, Vertex <G>>;

// The set of the evp.
template <typename G>
using evpset = std::set <evp <G>>;

// Return the upstream evp for the given node.
template <typename G>
evp <G>
get_upstream (const G &g, Vertex <G> cv)
{
  typename G::in_edge_iterator ei, ee;
  tie (ei, ee) = boost::in_edges (cv, g);
  // The set of links cannot be empty.
  assert (ei != ee);
  // We expect one edge only.
  assert (ei + 1 == ee);
  // The upstream edge.
  Edge <G> ue = *ei;
  // The the upstream vertex.
  Vertex <G> uv = boost::source (ue, g);
  return std::make_pair(ue, uv);
}

// Return the set of downstream evps for the given node.
template <typename G>
evpset <G>
get_downstream (const G &g, Vertex <G> v)
{
  evpset <G> s;

  typename G::out_edge_iterator ei, ee;
  for (tie (ei, ee) = out_edges (v, g); ei != ee; ++ei)
    {
      Edge<G> e = *ei;
      Vertex<G> t = boost::target(e, g);
      s.insert (evp <G> (e, t));
    }

  return s;
}

template<typename G>
double
fsp_onu (const G &g, Vertex <G> cn, Vertex <G> pn, Path <G> &p, v2lp <G> &r)
{
  assert (boost::get (boost::vertex_type, g, cn) == ONU);
  r[cn].push_back(r);
}

template <typename G>
double
fsp_arn (const G &g, Vertex <G> cn, Vertex <G> pn, Path <G> &p, v2lp <G> &r)
{
  assert (boost::get (boost::vertex_type, g, cn) == ARN);

  double availa = boost::get (boost::vertex_availa, g, cn);
  availa *= (1 - parallel_availa (g, cn, pn));
  return availa;
}

template <typename G>
std::pair <double, double>
trace_prns (const G &g, Vertex <G> cn, Vertex <G> pn)
{
  evp <G> p = get_upstream (g, cn);
  VERTEX_T nvt = boost::get (boost::vertex_type, g, p.second);

  if (nvt != PRN)
    {
      // It must be an OLT or an ARN.
      assert (nvt == OLT || nvt == ARN);

    }
  else
    ap = trace_prns (g, p.second, cn);

  // The availability of the upstream edge.
  ap.first *= boost::get (boost::edge_availa, g, p.first);
  // The availability of the current vertex.
  ap.first *= boost::get (boost::vertex_availa, g, cn);

  ap.second *= parallel_availa (g, cn, pn, false);

  return ap;
}

template <typename G>
double
fsp_prn (const G &g, Vertex <G> cn, Vertex <G> pn, Path <G> &p, v2lp <G> &r)
{
  assert (boost::get (boost::vertex_type, g, cn) == PRN);

  double availa;
  evp <G> p = get_upstream (g, cn);

  if (p.second == pn)
    {
      // The first case (see above), we are going downstream.
      availa = (1 - parallel_availa (g, cn, pn));
      availa *= boost::get (boost::vertex_availa, g, cn);
    }
  else
    { 
      // The second case (see above), we are climbing upstream.
      std::pair<double, double> ap = trace_prns (g, cn, pn);
      availa = ap.first * (1 - ap.second);
    }

  return availa;
}

// Breadth-first search, which takes into account PON.
template <typename G>
double
fsp (const G &g, Vertex <G> cn, Vertex <G> pn)
{
  VERTEX_T t = boost::get (boost::vertex_type, g, cn);

  if (t == ONU)
    availa = fsp_onu (g, cn, pn);
  else if (t == ICO)
    availa = boost::get (boost::vertex_availa, g, cn);
  else if (t == PRN)
    availa = calc_prn_availa (g, cn, pn);
  else if (t == ARN)
    availa = calc_arn_availa (g, cn, pn);
  else if (t == OLT)
    availa = boost::get (boost::vertex_availa, g, cn);
  else
    assert (false);

  return availa;
}

#endif /* BFS_HPP */
