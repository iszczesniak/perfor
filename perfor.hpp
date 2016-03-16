#ifndef PERFOR_HPP
#define PERFOR_HPP

#include "broker.hpp"
#include "graph.hpp"
#include "utils.hpp"

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
allocate (const G &g, const V2D <G> &req)
{
  broker<G> b(g);
  return b.service(req);
}

/**
 * Calculates the mean performance of ONUs, including ICOs.
 */
template <typename G>
double
calc_mean_perf (const G &g, const V2D <G> &req, const V2D <G> &all)
{
  // The performance accumulator.
  ba::accumulator_set <double, ba::stats <ba::tag::mean> > pa;

  for (const auto v: get_nodes (g, VERTEX_T::ONU, VERTEX_T::ICO))
    pa (get <G> (all, v) / get <G> (req, v));

  return ba::mean (pa);
}

/**
 * Calculates the mean connectivity of ONUs, including ICOs.
 */
template <typename G>
double
calc_mean_conn (const G &g)
{
  // The connectivity accumulator.
  ba::accumulator_set <double, ba::stats <ba::tag::mean> > ca;

  for (const auto v: get_nodes (g, VERTEX_T::ONU, VERTEX_T::ICO))
    ca (boost::get(boost::vertex_paths, g, v).size ());

  return ba::mean (ca);
}

#endif /* PERFOR_HPP */
