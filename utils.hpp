#ifndef UTILS_HPP
#define UTILS_HPP

#include "graph.hpp"

#include <iomanip>
#include <iostream>

template<typename G>
int
count_node_types(const G &g, VERTEX_T t)
{
  int count = 0;
  Viter<G> vi, ve;
  for (tie(vi, ve) = vertices(g); vi != ve; ++vi)
    count += (boost::get(boost::vertex_type, g, *vi) == t);
  return count;
}

/// Prints out the stage for the current node, cn, ignoring the
/// previous node, pn.
template<typename G>
void
print_pon(const G &g, std::ostream &out, Vertex<G> cn)
{
  const static char *names[] = {"OLT", "ONU", "ICO",
                              "PRN", "ARN"};

  out << names [boost::get (boost::vertex_type, g, cn)]
      << " (" << boost::get (boost::vertex_name, g, cn)
      << ") availability = "
      << boost::get(boost::vertex_availa, g, cn)
      << std::endl;

  typename G::out_edge_iterator ei, ee;
  for (tie (ei, ee) = out_edges (cn, g); ei != ee; ++ei)
    {
      Edge<G> e = *ei;
      Vertex<G> nn = boost::target(e, g);

      out << "Edge to ("
          << boost::get (boost::vertex_name, g, nn)
          << ") availability = "
          << boost::get(boost::edge_availa, g, e)
          << std::endl;

      print_pon(g, out, nn);
    }
}

template<typename G>
void
print_pon(const G &g, std::ostream &out)
{
  Vertex<G> olt = *vertices(g).first;
  assert(boost::get(boost::vertex_type, g, olt) == OLT);
  print_pon(g, out, olt);
}

#endif /* UTILS_HPP */
