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

  // The available bitrate for links.
  std::map <Edge <G>, double> available;

public:
  broker(const G &g): m_g (g)
  {
  }

  // Service the nodes now.
  V2D <G>
  service (const V2D <G> &req)
  {
    // Nodes that need service.
    std::deque <Vertex <G> > nodes;

    for (const auto p: req)
      nodes.push_back(p.first);

    V2D <G> all;

    for(const auto v: nodes)
      all[v] = service(v, get <G> (req, v));

    return all;
  }

private:
  // Service a single node.
  double
  service(Vertex<G> v, double req)
  {
    return 0;
  }

  void
  sort(std::deque <Vertex <G> > &nodes)
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
