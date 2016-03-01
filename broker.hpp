#ifndef BROKER_HPP
#define BROKER_HPP

#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/visitors.hpp>

#include <list>
#include <map>
#include <tuple>

template <typename G>
class broker
{
  // The Vertex to list of paths.
  typedef typename std::map <Vertex <G>, std::list<Path <G> > > v2lp;

  // The shortest paths.
  v2lp paths;

  // Nodes that need service.
  std::list <Vertex <G> > nodes;

  // The graph we're working on.
  const G &m_g;

public:
  broker(const G &g): m_g (g)
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

        // Find a path to a given target: ONU or ICO.
        for(const auto t: get_nodes (g, VERTEX_T::ONU, VERTEX_T::ICO))
          {
            auto p = trace_path (g, pred, t);
            paths[t].push_back(p);
          }
      }
  }

  void
  push (Vertex<G> v)
  {
    nodes.push_back (v);
  }

  void
  service ()
  {
  }

private:
  // Trace path
  template <typename M>
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
};

#endif /* BROKER_HPP */
