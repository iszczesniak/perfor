#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <deque>
#include <list>
#include <set>
#include <string>

#include <boost/optional.hpp>
#include <boost/graph/adjacency_list.hpp>

// The set of ONUs.
typedef std::set<int> SOO;

// Vertex types:
// OLT - well, OLT
// ONU - well, well, ONU
// ICO - IC ONU (Interoperator Communication ONU)
// ARN - active remote node
// PRN - passive remote node
enum class VERTEX_T {OLT, ONU, ICO, PRN, ARN};

enum class EDGE_T {UNDEF, DOWN, UP};

namespace boost {
  // Vertex type.
  enum vertex_type_t {vertex_type};
  // Downstream datarate requested for vertex.
  enum vertex_dr_req_t {vertex_dr_req};
  // Downstream datarate allocated for vertex.
  enum vertex_dr_all_t {vertex_dr_all};
  // Edge type.
  enum edge_type_t {edge_type};
  // ONUs allocated on the edge.
  enum edge_onus_t {edge_onus};
  // Available datarate of an edge.
  enum edge_rate_t {edge_rate};
  BOOST_INSTALL_PROPERTY(vertex, type);
  BOOST_INSTALL_PROPERTY(vertex, dr_req);
  BOOST_INSTALL_PROPERTY(vertex, dr_all);
  BOOST_INSTALL_PROPERTY(edge, type);
  BOOST_INSTALL_PROPERTY(edge, onus);
  BOOST_INSTALL_PROPERTY(edge, rate);
}

/**
 * The type of the graph we use.
 */
typedef
boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS,
                      boost::property<boost::vertex_name_t, std::string,
                      boost::property<boost::vertex_type_t, VERTEX_T,
                      boost::property<boost::vertex_dr_req_t, double,
                      boost::property<boost::vertex_dr_all_t, double> > > >,
                      boost::property<boost::edge_type_t, EDGE_T,
                      boost::property<boost::edge_rate_t, double,
                      boost::property<boost::edge_onus_t, SOO> > > >
graph;

typedef graph::edge_descriptor edge;
typedef graph::vertex_descriptor vertex;

template <typename G>
using Edge = typename G::edge_descriptor;

template <typename G>
using Viter = typename G::vertex_iterator;

template <typename G>
using Vertex = typename G::vertex_descriptor;

template <typename G>
using Path = std::list <Edge<G> >;

template <typename G>
using v2lp = std::map <Vertex <G>, std::deque<Path <G> > >;

std::string
to_string (VERTEX_T t);

#endif /* GRAPH_HPP */
