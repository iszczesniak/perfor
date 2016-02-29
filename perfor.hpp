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

// Calculate performance values for all ONUs.
template <typename G>
void
calc_perfors (const G &g, vmap <G> &va)
{
  // Iterate over every ONU, and find all its paths.
  for(const auto v: get_nodes (g, VERTEX_T::OLT, VERTEX_T::ICO))
    {
      // The source vertex.
      Vertex<G> s = v;
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

  for(const auto v: get_nodes (g, VERTEX_T::ONU, VERTEX_T::ICO))
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
