#ifndef UTILS_TRAGEN_HPP
#define UTILS_TRAGEN_HPP

#include "args.hpp"
#include "graph.hpp"
#include "utils.hpp"

#include <iostream>

// This function generates the downstream traffic, i.e. to ONUs.  The
// fraction uv (utilization value) of the total downstream rate is
// split among all ONUS, reagardless whether they are IC or not.

template<typename G, typename T>
void
generate_traffic(G &g, args &a, T &gen, double uv)
{
  assert(num_vertices(g) != 0);

  // Count the number of ONUs, including the IC-ONUs.
  int onus = count_node_types(g, VERTEX_T::ONU);
  onus += count_node_types(g, VERTEX_T::ICO);

  // The bitrate for ONU.
  double bfo = a.drate * uv / onus;

  Viter<G> vi, ve;
  for (tie(vi, ve) = vertices(g); vi != ve; ++vi)
    {
      VERTEX_T vt = boost::get(boost::vertex_type, g, *vi);

      if (vt == VERTEX_T::ONU || vt == VERTEX_T::ICO)
        boost::get(boost::vertex_drate, g, *vi) = bfo; 
    }
}

#endif /* UTILS_TRAGEN_HPP */
