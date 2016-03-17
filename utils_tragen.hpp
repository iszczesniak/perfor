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
  assert(get(boost::edge_type, m_g, oe) == DIR_T::DOWN);
  return get(boost::edge_rate, m_g, oe);
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
  assert(get(boost::edge_type, m_g, ie) == DIR_T::UP);
  return get(boost::edge_rate, m_g, ie);
}

// This function generates the downstream traffic, i.e. to ONUs.  The
// fraction uv (utilization value) of the total downstream rate is
// split among all ONUS, reagardless whether they are IC or not.
template<typename G>
V2D <G>
generate_traffic(const G &g, double bfo)
{
  V2D <G> req;
  
  assert(num_vertices(g) != 0);

  for (auto v: get_nodes(g, VERTEX_T::ONU, VERTEX_T::ICO))
    req[v] = bfo; 

  return req;
}

#endif /* UTILS_TRAGEN_HPP */
