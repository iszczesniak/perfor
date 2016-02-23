#ifndef UTILS_NETGEN_HPP
#define UTILS_NETGEN_HPP

#include "args.hpp"
#include "graph.hpp"
#include "utils.hpp"

#include <iomanip>
#include <random>
#include <sstream>

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
  for (tie(vi, ve) = vertices(g); vi != ve; ++vi)
    {
      Vertex<G> v = *vi;
      std::ostringstream out;
      out << "v" << std::setw(width) << std::setfill('0') << count++;
      boost::get(boost::vertex_name, g, v) = out.str();
    }
}

// We start counting stages from 0. pn is the previous node.
template<typename G, typename T>
void
generate_further (G &g, args &a, T &gen, int stage, Vertex<G> pn)
{
  std::bernoulli_distribution qd(a.q);
  std::bernoulli_distribution sd(a.s);

  // This is the first node.  It can be an ONU or a RN.
  Vertex<G> n = add_vertex(g);
  // The first fiber.
  bool status = add_edge(pn, n, g).second;
  assert(status);

  // The type of the first node.
  VERTEX_T nt;

  if (stage == a.stages)
    nt = VERTEX_T::ONU;
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

template<typename G, typename T>
void
generate_pon(G &g, args &a, T &gen)
{
  assert(num_vertices(g) == 0);
  Vertex<G> olt = add_vertex(g);
  boost::get(boost::vertex_type, g, olt) = VERTEX_T::OLT;
  generate_further(g, a, gen, 0, olt);
}

template<typename G, typename T>
void
interconnect_pons (G &pon1, G &pon2, double r, T &gen)
{
  // The number of ONUs in pon1.
  int onus = count_node_types(pon1, VERTEX_T::ONU);
  // The number of ICOs in pon1 to generate.
  int icos = onus * r;

  // The set of ONUs in pon1.
  auto soo1 = get_nodes (pon1, VERTEX_T::ONU);
  // The set of ONUs in pon2.
  auto soo2 = get_nodes (pon2, VERTEX_T::ONU);

  for (int i = 0; i < icos; ++i)
    {
      Vertex<G> v1 = get_random_element (soo1, gen);
      Vertex<G> v2 = get_random_element (soo2, gen);
      
      soo1.erase (v1);
      soo2.erase (v2);
    }
}

#endif /* UTILS_NETGEN_HPP */
