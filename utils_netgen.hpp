#ifndef UTILS_NETGEN_HPP
#define UTILS_NETGEN_HPP

#include "args.hpp"
#include "graph.hpp"

#include <iomanip>
#include <random>
#include <sstream>

/**
 * Names the vertices.
 */
template<typename G>
void
name_vertices(G &g)
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
generate_further(G &g, args &a, T &gen, int stage, Vertex<G> pn)
{
  std::bernoulli_distribution qd(a.q);
  std::bernoulli_distribution rd(a.r);
  std::bernoulli_distribution sd(a.s);

  // This is the first node.  It can be an ONU or a RN.
  Vertex<G> n = add_vertex(g);

  // The type of the first node.
  VERTEX_T nt;

  if (stage == a.stages)
    {
      // We reached the end.  It's not a stage, but just an ONU.
      // Choose the type of the ONU.
      nt = rd(gen) ? VERTEX_T::ICO : VERTEX_T::ONU;
    }
  else
    {
      // It's a new stage, and so set the type of the RN.
      nt = qd(gen) ? VERTEX_T::ARN : VERTEX_T::PRN;

      for (int i = 0; i < a.sratio; ++i)
        {
          int nstage = sd(gen) ? (stage + 1) : a.stages;
          generate_further(g, a, gen, nstage, n);
        }
    }

  boost::get(boost::vertex_type, g, n) = nt;
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

#endif /* UTILS_NETGEN_HPP */
