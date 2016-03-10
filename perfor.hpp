#ifndef PERFOR_HPP
#define PERFOR_HPP

#include "broker.hpp"
#include "graph.hpp"

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>

namespace ba = boost::accumulators;

// The type where the calculated performance values are stored.
template <typename G>
using v2d = std::map <Vertex <G>, double>;

// Calculate performance values for all ONUs.
template <typename G>
V2D <G>
allocate (G &g, const V2D &req)
{
  V2D <G> all;
  
  broker<G> b(g);

  // Tell the broker what nodes need service.
  for(const auto v: get_nodes (g, VERTEX_T::ONU, VERTEX_T::ICO))
    b.push(v);

  b.service();

  return all;
}

/**
 * Calculates the mean performance of ONUs, including ICOs.
 */
template <typename G, typename M>
double
calc_mean_perf (const G &g, const M &m)
{
  // The performance accumulator.
  ba::accumulator_set <double, ba::stats <ba::tag::mean> > pa;

  for(const auto v: get_nodes (g, VERTEX_T::ONU, VERTEX_T::ICO))
    pa (get (m, v));

  return ba::mean (pa);
}

/**
 * Calculates the mean connectivity of ONUs, including ICOs.
 */
template <typename G>
double
calc_mean_conn (const G &g)
{
  return 0;
}

#endif /* PERFOR_HPP */
