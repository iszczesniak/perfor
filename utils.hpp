#ifndef UTILS_HPP
#define UTILS_HPP

#include "graph.hpp"

#include <boost/range.hpp>

#include <iomanip>
#include <random>
#include <set>

template<typename G>
int
count_nodes(const G &g, VERTEX_T t)
{
  int count = 0;
  Viter<G> vi, ve;
  for (tie(vi, ve) = vertices(g); vi != ve; ++vi)
    count += (boost::get(boost::vertex_type, g, *vi) == t);
  return count;
}

template<typename G>
int
count_nodes(const G &g, VERTEX_T t1, VERTEX_T t2)
{
  int count = count_nodes(g, t1);
  count += count_nodes(g, t2);
  return count;
}

// The set of nodes of given type in a pon.
template<typename G>
std::set<Vertex<G> >
get_nodes (const G &g, VERTEX_T t)
{
  std::set<Vertex<G> > s;

  Viter<G> vi, ve;
  for (tie(vi, ve) = vertices (g); vi != ve; ++vi)
    if (boost::get (boost::vertex_type, g, *vi) == t)
      s.insert (*vi);

  return s;
}

template<typename G>
std::set<Vertex<G> >
get_nodes (const G &g, VERTEX_T t1, VERTEX_T t2)
{
  std::set<Vertex<G> > s1 = get_nodes (g, t1);
  std::set<Vertex<G> > s2 = get_nodes (g, t2);
  s1.insert (s2.begin (), s2.end ());
  return s1;
}

/**
 * Get a random element from a container.
 */
template <typename C, typename T>
typename C::value_type
get_random_element(const C &c, T &gen)
{
  assert(!c.empty());

  std::uniform_int_distribution<> dist (0, c.size() - 1);
  int value = dist (gen);
  typename C::const_iterator i = c.begin();
  std::advance(i, value);

  return *i;
}

/// Prints out the stage for the current node, cn, given the previous
/// node pn.
template<typename G>
void
print_pon(const G &g, std::ostream &out, Vertex<G> cn, Vertex<G> pn)
{
  for (const auto &e: make_iterator_range(out_edges (cn, g)))
    {
      Vertex<G> nn = boost::target(e, g);

      // Don't follow upstream.
      if (nn != pn)
        {
          out << to_string (boost::get (boost::vertex_type, g, cn))
              << " (" << boost::get (boost::vertex_name, g, cn)
              << ") -> " << to_string (boost::get (boost::vertex_type, g, nn))
              << " (" << boost::get (boost::vertex_name, g, nn)
              << ")" << std::endl;

          out << "Downstream: rate = "
              << boost::get (boost::edge_rate, g, e)
              << std::endl;

          // The reverse edge.
          Edge<G> re;
          bool rs;
          tie(re, rs) = boost::edge(nn, cn, g);
          assert(rs);
          
          out << "Upstream: rate = "
              << boost::get (boost::edge_rate, g, re)
              << std::endl;

          print_pon(g, out, nn, cn);
        }
    }
}

template<typename G>
void
print_pon(const G &g, std::ostream &out)
{
  Vertex<G> olt = *vertices(g).first;
  assert(boost::get(boost::vertex_type, g, olt) == VERTEX_T::OLT);
  print_pon(g, out, olt, G::null_vertex ());
}

template<typename G>
void
print_path(const G &g, const Path <G> &p, std::ostream &out)
{
  for (const auto e: p)
    {
      Vertex <G> s = source (e, g);
      Vertex <G> t = target (e, g);

      out << to_string (boost::get (boost::vertex_type, g, s))
          << " (" << boost::get (boost::vertex_name, g, s)
          << ") ->" << to_string (boost::get (boost::vertex_type, g, t))
          << " (" << boost::get (boost::vertex_name, g, t)
          << ")" << std::endl;
    }
}

#endif /* UTILS_HPP */
