#ifndef UTILS_TRAGEN_HPP
#define UTILS_TRAGEN_HPP

#include "args.hpp"
#include "graph.hpp"
#include "utils.hpp"

#include <iostream>

// Returns the down rate of the PON.
template<typename G>
double
get_drate(const G &g)
{
  Vertex<G> olt = *vertices(g).first;
  assert(boost::get(boost::vertex_type, g, olt) == VERTEX_T::OLT);
  assert(boost::out_degree(olt, g) == 1);
  Edge<G> oe = *(out_edges(olt, g).first);
  assert(get(boost::edge_type, g, oe) == DIR_T::DOWN);
  return get(boost::edge_rate, g, oe);
}

// Returns the up rate of the PON.
template<typename G>
double
get_urate(const G &g)
{
  Vertex<G> olt = *vertices(g).first;
  assert(boost::get(boost::vertex_type, g, olt) == VERTEX_T::OLT);
  assert(boost::in_degree(olt, g) == 1);
  Edge<G> ie = *(in_edges(olt, g).first);
  assert(get(boost::edge_type, g, ie) == DIR_T::UP);
  return get(boost::edge_rate, g, ie);
}

// This function generates the downstream traffic, i.e. to ONUs.  The
// fraction uv (utilization value) of the total downstream rate is
// split among all ONUS, reagardless whether they are IC or not.
template<typename G>
V2D <G>
generate_traffic(const G &g, double uv)
{
  V2D <G> req;

  // The downstream bitrate.
  double drate = get_drate(g);

  // The number of ONUS among which to split the uv fraction of drate.
  int onus = count_nodes(g, VERTEX_T::ONU, VERTEX_T::ICO);

  // Bitrate for an ONU.
  double bfo = drate * uv / onus;

  assert(num_vertices(g) != 0);

  for (auto v: get_nodes(g, VERTEX_T::ONU, VERTEX_T::ICO))
    req[v] = bfo; 

  return req;
}

#endif /* UTILS_TRAGEN_HPP */
