#ifndef BROKER_HPP
#define BROKER_HPP

#include "fsp.hpp"
#include "graph.hpp"

#include <deque>
#include <limits>
#include <map>
#include <tuple>

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
    double drate = get_drate(m_g);
    double urate = get_urate(m_g);

    for (const auto &e: make_iterator_range(edges(m_g)))
      {
        DIR_T t = get(boost::edge_type, m_g, e);
        assert(t == DIR_T::UP || t == DIR_T::DOWN);
        available[e] = (t == DIR_T::DOWN) ? drate : urate;
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
      all[v] = service(v, req.at(v));

    return all;
  }

private:
  // Service a single node.  Return the allocated bitrate.  We try to
  // allocate the bitrate along the shortest paths, and stop searching
  // for a path when the requested bitrate is found.
  double
  service(Vertex<G> v, double req)
  {
    Path<G> empty_path;
    Path<G> &path = empty_path;

    // The bitrate found to allocate.
    double all = 0;

    // Iterate over the paths of the vertex.
    for (auto const &p: boost::get(boost::vertex_paths, m_g, v))
      {
        // The path bitrate.
        double pbr = find_path_bitrate(p);

        if (pbr > all)
          {
            all = pbr;
            path = p;
          }

        // We stop the search when sufficient bitrate was found.
        if (all >= req)
          break;
      }

    // We don't want to allocate more than requested.
    all = std::min (req, all);
    
    // Allocate the bitrate on all edges of the chosen path.
    for (auto const &e: path)
      available[e] -= all;

    return all;
  }

  // Return the bitrate a path can support.
  double
  find_path_bitrate (const Path<G> &p)
  {
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
