#ifndef BROKER_HPP
#define BROKER_HPP

#include "fsp.hpp"
#include "graph.hpp"

#include <deque>
#include <map>
#include <tuple>
#include <iostream>

template <typename G>
class broker
{
  // Nodes that need service.
  std::deque <Vertex <G> > nodes;

  // The graph we're working on.
  const G &m_g;

public:
  broker(const G &g): m_g (g)
  {
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
  }

  void
  sort()
  {
    struct ver_cmp
    {
      const G &m_g;

      ver_cmp(const G &g): m_g(g)
      {
      }

      bool operator () (Vertex<G> a, Vertex<G> b)
      {
        const auto &ap = boost::get (boost::vertex_paths, m_g, a);
        const auto &bp = boost::get (boost::vertex_paths, m_g, b);

        return ap.size() < bp.size();
      }
    };

    std::sort(nodes.begin(), nodes.end(), ver_cmp(m_g));
  }
};

#endif /* BROKER_HPP */
