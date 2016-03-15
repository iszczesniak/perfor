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
  // The graph we're working on.
  const G &m_g;

  // Nodes that need service.
  std::deque <Vertex <G> > nodes;

  // The requested
  V2D <G> req;
  
public:
  broker(const G &g): m_g (g)
  {
  }

  // Insert a node that need service, and what bitrate is requested.
  void
  push (Vertex<G> v, double br)
  {
    nodes.push_back (v);
  }

  // Service the nodes now.
  V2D <G>
  service ()
  {
    V2D <G> all;

    sort();
    for(const auto v: nodes)
      service(v);
    nodes.clear();

    return all;
  }

private:
  // Service a single node.
  double
  service(Vertex<G> v)
  {
    return 0;
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
