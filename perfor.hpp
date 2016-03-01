#ifndef PERFOR_HPP
#define PERFOR_HPP

#include "args.hpp"
#include "graph.hpp"

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>

#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/visitors.hpp>

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
  auto im = get (boost::vertex_index_t(), g);
  std::vector<Vertex<G> > pred_vec (num_vertices (g));
  auto pred = make_iterator_property_map (pred_vec.begin(), im);

  // Iterate over OLT and ICOs, and find paths to all other nodes.
  for(const auto s: get_nodes (g, VERTEX_T::OLT, VERTEX_T::ICO))
    {
      put (pred, s, s);
      auto rp = boost::record_predecessors (pred, boost::on_tree_edge ());
      auto vis = boost::make_bfs_visitor (rp);
      boost::breadth_first_search (g, s, boost::visitor (vis));

      // Find a path to a given ONU or ICO.
      for(const auto t: get_nodes (g, VERTEX_T::OLT, VERTEX_T::ICO))      
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
