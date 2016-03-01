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
#include <tuple>

namespace ba = boost::accumulators;

// The type where the calculated performance values are stored.
template <typename G>
using v2d = std::map <Vertex <G>, double>;

// Trace path
template <typename G, typename M>
Path<G>
trace_path (const G &g, const M &m, Vertex<G> t)
{
  Path<G> path;

  Vertex<G> c = t;

  while(true)
    {
      // Predecessing node.
      Vertex<G> p = get(m, c);

      if (p == c)
        break;

      Edge<G> e;
      bool s;
      std::tie(e, s) = boost::edge(p, c, g);
      assert(s);
      path.push_front(e);

      c = p;
    }

  return path;
}

// Calculate performance values for all ONUs.
template <typename G>
void
calc_perfors (const G &g, v2d <G> &va)
{
  // The Vertex to list of paths.
  typedef typename std::map <Vertex <G>, std::list<Path <G> > > v2lp;

  // The shortest paths.
  v2lp paths;

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

      // Find a path to a given target: ONU or ICO.
      for(const auto t: get_nodes (g, VERTEX_T::ONU, VERTEX_T::ICO))
        {
          auto p = trace_path (g, pred, t);
          paths[t].push_back(p);
        }
    }
}

// Calculate the mean performance for all ONUs, and make sure that the
// performance values have been calculated for all ONUs.
template <typename G>
double
mean_perfor (const G &g, const v2d <G> &p)
{
  // The performance accumulator.
  ba::accumulator_set <double, ba::stats <ba::tag::mean> > pa;

  for(const auto v: get_nodes (g, VERTEX_T::ONU, VERTEX_T::ICO))
    {
      // We want to make sure that all ONUs have the performance.
      typename v2d <G>::const_iterator i = p.find (v);
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
  v2d <G> p;
  calc_perfors (g, p);
  return mean_perfor (g, p);
}

#endif /* PERFOR_HPP */
