#ifndef UTILS_TRAGEN_HPP
#define UTILS_TRAGEN_HPP

#include "args.hpp"
#include "graph.hpp"
#include "utils.hpp"

// This function generates the load.

template<typename G, typename T>
void
generate_traffic(const G &g, args &a, T &gen, double uv)
{
  assert(num_vertices(g) == 0);

  // Count the number of ONUs, including the IC-ONUs.
  int onus = count_node_types(g, ONU);
  onus += count_node_types(g, ICO);
}

#endif /* UTILS_TRAGEN_HPP */
