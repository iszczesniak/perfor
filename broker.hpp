#ifndef BROKER_HPP
#define BROKER_HPP

// We're including the filtered graph before fsp.hpp, because
// otherwise the compilation fails.  I'm not sure why.
#include <boost/graph/filtered_graph.hpp>

#include "fsp.hpp"
#include "graph.hpp"

#include <deque>
#include <map>
#include <tuple>
#include <iostream>

template <typename G>
class broker
{
  // The type of the set of vertexes.
  typedef std::set <Vertex <G> > vset;

  // The type of the filtered graph.
  typedef boost::vertex_subset_complement_filter<G, vset>::type fg_t;

  // The shortest paths.
  v2lp <G> m_paths;

  // Nodes that need service.
  std::deque <Vertex <G> > nodes;

  // The graph we're working on.
  const G &m_g;

public:
  broker(const G &g): m_g (g)
  {
    // The set of excluded vertexes.
    vset ev;
    // The vertex predicate.
    boost::is_not_in_subset <vset> vp (ev);
    // The filtered graph.
    fg_t fg (m_g, boost::keep_all (), vp);

    // Iterate over OLT and ICOs, and find paths to all other nodes.
    for(auto s: get_nodes (g, VERTEX_T::OLT, VERTEX_T::ICO))
      {
        fsp <fg_t> {fg, s, m_paths};
        ev.insert (s);
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
      const v2lp <G> &m_paths;

      ver_cmp(const v2lp <G> &paths): m_paths(paths)
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
};

#endif /* BROKER_HPP */
