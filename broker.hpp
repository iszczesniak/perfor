#ifndef BROKER_HPP
#define BROKER_HPP

#include "fsp.hpp"
#include "graph.hpp"

#include <deque>
#include <limits>
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
    // Initialize the available bitrate.
    Vertex<G> olt = *vertices(g).first;
    assert(boost::get(boost::vertex_type, m_g, olt) == VERTEX_T::OLT);
    assert(boost::out_degree(olt, m_g) == 1);
    assert(boost::in_degree(olt, m_g) == 1);
    Edge<G> oe = *(out_edges(olt, m_g).first);
    Edge<G> ie = *(in_edges(olt, m_g).first);
    assert(get(boost::edge_type, m_g, oe) == DIR_T::DOWN);
    assert(get(boost::edge_type, m_g, ie) == DIR_T::UP);
    double db = get(boost::edge_rate, m_g, oe);
    double ub = get(boost::edge_rate, m_g, ie);

    for (const auto &e: make_iterator_range(edges(g)))
      {
        DIR_T t = get(boost::edge_type, m_g, e);
        assert(t == DIR_T::UP || t == DIR_T::DOWN);
        available[e] = (t == DIR_T::DOWN) ? db : ub;
      }
  }

  // Service the nodes now.
  V2D <G>
  service (const V2D <G> &req)
  {
    // Nodes that need service.
    std::deque <Vertex <G> > nodes;
    for (const auto p: req)
      nodes.push_back(p.first);
    sort(nodes);

    V2D <G> all;

    for(const auto v: nodes)
      all[v] = service(v, get <G> (req, v));

    return all;
  }

private:
  // Service a single node.  Return the allocated bitrate.
  double
  service(Vertex<G> v, double req)
  {
    return 0;
  }

  // Return the bitrate a path can support.
  double
  find_path_bitrate (const Path<G> &p)
  {
    assert(!p.empty());
    double bitrate = std::numeric_limits<double>::max();

    for (const auto e: p)
      bitrate = std::min(bitrate, available[e]);

    return bitrate;
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
    assert(std::is_sorted (nodes.begin(), nodes.end(), ver_cmp(m_g)));
  }
};

#endif /* BROKER_HPP */
