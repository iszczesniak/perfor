#ifndef BROKER_HPP
#define BROKER_HPP

#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/visitors.hpp>

#include <deque>
#include <map>
#include <tuple>
#include <iostream>

template <typename G>
class broker
{
  // The Vertex to list of paths.
  typedef typename std::map <Vertex <G>, std::list<Path <G> > > v2lp;

  // The shortest paths.
  v2lp m_paths;

  // Nodes that need service.
  std::deque <Vertex <G> > nodes;

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
            m_paths[t].push_back(p);
          }
      }
  }

  // Insert nodes that need service.
  void
  push (Vertex<G> v)
  {
    nodes.push_back (v);
  }

  // Service the nodes now.
  void
  service ()
  {
    sort();
    for(const auto v: nodes)
      service(v);
    nodes.clear();
  }

private:
  // Service a single node.
  void
  service(Vertex<G> v)
  {
    auto i = m_paths.find(v);
    assert(i != m_paths.end());
    std::cout << "servicing node with the number of paths = "
              << i->second.size () << std::endl;
  }

  void
  sort()
  {
    struct ver_cmp
    {
      const v2lp &m_paths;

      ver_cmp(const v2lp &paths): m_paths(paths)
      {
      }

      bool operator () (Vertex<G> a, Vertex<G> b)
      {
        auto ai = m_paths.find(a);
        assert(ai != m_paths.end());
        auto bi = m_paths.find(b);
        assert(bi != m_paths.end());

        return ai->second.size() < bi->second.size();
      }
    };

    std::sort(nodes.begin(), nodes.end(), ver_cmp(m_paths));
  }
  
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
