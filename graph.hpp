#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <deque>
#include <list>
#include <map>
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

// Direction type.
enum class DIR_T {UNDEF, DOWN, UP};

template <typename G>
using Edge = typename G::edge_descriptor;

template <typename G>
using Viter = typename G::vertex_iterator;

template <typename G>
using Vertex = typename G::vertex_descriptor;

template <typename G>
using Path = std::list <Edge <G> >;

template <typename G>
using Paths = std::deque <Path <G> >;

// The type of the set of vertexes.
template <typename G>
using Vset = std::set <Vertex <G> >;

// The vertex to double map.
template <typename G>
using V2D = std::map <Vertex <G>, double>;

// The map of vertex to a deque of paths.
template <typename G>
using v2lp = std::map <Vertex <G>, Paths <G> >;

namespace boost {
  // Vertex type.
  enum vertex_type_t {vertex_type};
  // Vertex paths.
  enum vertex_paths_t {vertex_paths};
  // Edge type.
  enum edge_type_t {edge_type};
  // Available datarate of an edge.
  enum edge_rate_t {edge_rate};
  BOOST_INSTALL_PROPERTY(vertex, type);
  BOOST_INSTALL_PROPERTY(vertex, paths);
  BOOST_INSTALL_PROPERTY(edge, type);
  BOOST_INSTALL_PROPERTY(edge, rate);
}

typedef
boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::bidirectionalS>
GraphTmp;

/**
 * The type of the graph we use.
 */
typedef
boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS,
                      boost::property<boost::vertex_name_t, std::string,
                      boost::property<boost::vertex_type_t, VERTEX_T,
                      boost::property<boost::vertex_paths_t, Paths <GraphTmp> > > >,
                      boost::property<boost::edge_type_t, DIR_T,
                      boost::property<boost::edge_rate_t, double> > >
graph;

typedef graph::edge_descriptor edge;
typedef graph::vertex_descriptor vertex;

std::string
to_string (VERTEX_T t);

#endif /* GRAPH_HPP */
