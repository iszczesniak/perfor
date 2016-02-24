#ifndef GRAPH_HPP
#define GRAPH_HPP

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

namespace boost {
  // Vertex type.
  enum vertex_type_t {vertex_type};
  // Downstream datarate required for vertex.
  enum vertex_drate_t {vertex_drate};
  // ONUs allocated downstream on the edge.
  enum edge_donus_t {edge_donus};
  // ONUs allocated upstream on the edge.
  enum edge_uonus_t {edge_uonus};
  // Available downstream datarate of an edge.
  enum edge_drate_t {edge_drate};
  // Available upstream datarate of an edge.
  enum edge_urate_t {edge_urate};
  BOOST_INSTALL_PROPERTY(vertex, type);
  BOOST_INSTALL_PROPERTY(vertex, drate);
  BOOST_INSTALL_PROPERTY(edge, donus);
  BOOST_INSTALL_PROPERTY(edge, uonus);
  BOOST_INSTALL_PROPERTY(edge, drate);
  BOOST_INSTALL_PROPERTY(edge, urate);
}

/**
 * The type of the graph we use.
 */
typedef
boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS,
                      boost::property<boost::vertex_name_t, std::string,
                      boost::property<boost::vertex_type_t, VERTEX_T,
                      boost::property<boost::vertex_drate_t, double> > >,
                      boost::property<boost::edge_drate_t, double,
                      boost::property<boost::edge_urate_t, double,
                      boost::property<boost::edge_donus_t, SOO,
                      boost::property<boost::edge_uonus_t, SOO> > > > >
graph;

typedef graph::edge_descriptor edge;
typedef graph::vertex_descriptor vertex;

template <typename G>
using Edge = typename G::edge_descriptor;

template <typename G>
using Viter = typename G::vertex_iterator;

template <typename G>
using Vertex = typename G::vertex_descriptor;

#endif /* GRAPH_HPP */
