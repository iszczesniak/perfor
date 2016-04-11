#ifndef UTILS_NETGEN_HPP
#define UTILS_NETGEN_HPP

#include "args_net.hpp"
#include "fsp.hpp"
#include "graph.hpp"
#include "utils.hpp"

#include <iomanip>
#include <random>
#include <sstream>
#include <tuple>

/**
 * Names the vertices.
 */
template<typename G>
void
name_vertices (G &g)
{
  int count = 1;

  int number = boost::num_vertices(g);
  int width = int(log10(number)) + 1;

  Viter<G> vi, ve;
  for (std::tie(vi, ve) = vertices(g); vi != ve; ++vi)
    {
      Vertex<G> v = *vi;
      std::ostringstream out;
      out << "v" << std::setw(width) << std::setfill('0') << count++;
      boost::get(boost::vertex_name, g, v) = out.str();
    }
}

template<typename G, typename T>
void
add_edge (G &g, Vertex<G> pn, Vertex<G> n, T rate, DIR_T type)
{
  bool s;
  Edge<G> e;
  tie(e, s) = boost::add_edge(pn, n, g);
  assert(s);
  boost::get(boost::edge_rate, g, e) = rate;
  boost::get(boost::edge_type, g, e) = type;
}

// We start counting stages from 0. pn is the previous node.
template<typename G, typename T>
void
generate_further (G &g, const args_net<double> &a, T &gen, int stage, Vertex<G> pn)
{
  std::bernoulli_distribution qd(a.q);
  std::bernoulli_distribution rd(a.r);
  std::bernoulli_distribution sd(a.s);

  // This is the first node.  It can be an ONU or a RN.
  Vertex<G> n = add_vertex(g);

  // The downstream edge.
  add_edge (g, pn, n, a.drate, DIR_T::DOWN);
  // The upstream edge.
  add_edge (g, n, pn, a.urate, DIR_T::UP);
  
  // The type of the first node.
  VERTEX_T nt;

  if (stage == a.stages)
    // We reached the end.  It's not a stage, but just an ONU.  Choose
    // the type of the ONU.
    nt = rd(gen) ? VERTEX_T::ICO : VERTEX_T::ONU;
  else
    {
      // It's a new stage, and so set the type of the RN.
      nt = qd(gen) ? VERTEX_T::ARN : VERTEX_T::PRN;

      for (int i = 0; i < a.sratio; ++i)
        {
          int nstage = sd (gen) ? (stage + 1) : a.stages;
          generate_further (g, a, gen, nstage, n);
        }
    }

  boost::get (boost::vertex_type, g, n) = nt;
}

template<typename G>
G
generate_pon (const args_net<double> &a)
{
  G pon;
  assert(num_vertices(pon) == 0);
  std::mt19937 gen;
  gen.seed(a.seed);
  Vertex<G> olt = add_vertex(pon);
  boost::get(boost::vertex_type, pon, olt) = VERTEX_T::OLT;
  generate_further(pon, a, gen, 0, olt);
  name_vertices(pon);
  fill_paths(pon);
  return pon;
}

#endif /* UTILS_NETGEN_HPP */
