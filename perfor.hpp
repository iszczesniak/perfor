#ifndef PERFOR_HPP
#define PERFOR_HPP

#include "args.hpp"
#include "graph.hpp"

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>

#include <iomanip>
#include <map>
#include <random>
#include <sstream>

namespace ba = boost::accumulators;

// The type where the calculated performance values are stored.
template <typename G>
using vmap = std::map <Vertex <G>, double>;

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

// Calculate the product which is used to calculate the parallel
// availability for the node cv and all neighbour nodes except node pv
// and the upstream node (if requested).  The parallel availability is
// then: (1 - product).
template <typename G>
double
parallel_availa (const G &g, Vertex <G> cv, Vertex <G> pv, bool fu = true)
{
  // The set of evps to consider.
  evpset <G> s = get_downstream (g, cv);
  if (fu)
    s.insert (get_upstream (g, cv));

  double product = 1;
  for (const auto &p: s)
    if (p.second != pv)
      {
        // Edge evailability.
        double ea = boost::get (boost::edge_availa, g, p.first);
        // Next vertex availability.
        double nva = calc_availa (g, p.second, cv);
        product *= (1 - ea * nva);
      }

  return product;
}

// This function is called in two cases.  The first case is when we
// want to calculate the availability for the given ONU.  The second
// case is when we reach an ONU while calculating the availability for
// a different ONU.
template<typename G>
double
process_onu (const G &g, Vertex <G> cn, Vertex <G> pn)
{
  assert (boost::get (boost::vertex_type, g, cn) == ONU);

  if (pn == G::null_vertex ())
    {
      // The first case (see above).
      evp <G> p = get_upstream (g, cn);
      double availa = boost::get (boost::vertex_availa, g, cn);
      availa *= boost::get (boost::edge_availa, g, p.first);
      availa *= calc_availa (g, p.second, cn);
      return availa;
    }
  else
    // The second case (see above).
    return 0;
}

// Calculate the availability for the given ARN.  Don't consider the
// pn vertex.
template <typename G>
double
process_arn (const G &g, Vertex <G> cn, Vertex <G> pn)
{
  assert (boost::get (boost::vertex_type, g, cn) == ARN);

  double availa = boost::get (boost::vertex_availa, g, cn);
  availa *= (1 - parallel_availa (g, cn, pn));
  return availa;
}

// Returns a pair of availabilities.
template <typename G>
std::pair <double, double>
trace_prns (const G &g, Vertex <G> cn, Vertex <G> pn)
{
  std::pair <double, double> ap;

  evp <G> p = get_upstream (g, cn);
  VERTEX_T nvt = boost::get (boost::vertex_type, g, p.second);

  if (nvt != PRN)
    {
      // It must be an OLT or an ARN.
      assert (nvt == OLT || nvt == ARN);
      // The availability of the upstream vertex.
      double nva = boost::get (boost::vertex_availa, g, p.second);
      // This is the recursive availability of the next vertex divided
      // by its availability.  The nva is accounted for in the first
      // element of the availability probability.
      double dnva = calc_availa (g, p.second, cn) / nva;
      ap.first = nva;
      // This is the first element in the product of the parallel
      // availabilities.
      ap.second = (1 - dnva);
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

// There are two cases we deal with in this function.  The first case
// is when we reach this node from an upstream node.  The second case
// when we reach this node from a downstream node.
template <typename G>
double
find_prn_availa (const G &g, Vertex <G> cn, Vertex <G> pn)
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

template <typename G>
double
process_olt (const G &g, Vertex <G> cn, Vertex <G> pn)
{
  assert (boost::get (boost::vertex_type, g, cn) == PRN);
}

// Calculate the availability for the given node.
template <typename G>
double
find_paths (const G &g, Vertex <G> cn, Vertex <G> pn)
{
  VERTEX_T t = boost::get (boost::vertex_type, g, cn);

  if (t == ONU)
    availa = process_onu (g, cn, pn);
  else if (t == ICO)
    availa = process_ico (g, cn, pn);
  else if (t == PRN)
    availa = process_prn (g, cn, pn);
  else if (t == ARN)
    availa = process_arn (g, cn, pn);
  else if (t == OLT)
    availa = process_olt (g, cn, pn);
  else
    abort ();

  return availa;
}

// Calculate performance values for all ONUs.
template <typename G>
void
calc_perfors (const G &g, vmap <G> &va)
{
  auto onus = get_onus (g);

  // Iterate over every ONU, and find its paths.
  for(const auto v: get_onus (g))
    {
      
    }

  // Choose a path for an ONU, and calculate its performance.
  for(const auto v: onus)
    {

    }
}

// Calculate the mean performance for all ONUs, and make sure that the
// performance values have been calculated for all ONUs.
template <typename G>
double
mean_perfor (const G &g, const vmap <G> &p)
{
  // The performance accumulator.
  ba::accumulator_set <double, ba::stats <ba::tag::mean> > pa;

  for(const auto v: get_onus (g))
    {
      // We want to make sure that all ONUs have the performance.
      typename vmap <G>::const_iterator i = p.find (v);
      assert (i != p.end ());
      pa (i->second);
    }

  return ba::mean (pa);
}

/**
 * Calculates the mean performance of ONUs, including ICOs.
 */
template <typename G>
double
calc_mean_perfor (const G &g)
{
  // Vertex performance.
  vmap <G> p;
  calc_perfors (g, p);
  return mean_perfor (g, p);
}

#endif /* PERFOR_HPP */
