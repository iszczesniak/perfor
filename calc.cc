#include "calc.hpp"

#include "graph.hpp"
#include "simulation.hpp"
#include "utils_netgen.hpp"
#include "utils_tragen.hpp"

#include <random>

calc::calc(simulation &sim, const args &a, int seed, double uv):
  m_sim (sim), m_a (a), m_seed (seed), m_uv (uv)
{
}

void
calc::operator()()
{
  // Random number generator.
  std::mt19937 gen (m_seed);

  // Generate PON.
  graph pon1, pon2;
  generate_pon (pon1, m_a, gen);
  generate_pon (pon2, m_a, gen);

  // Interconnect PONs.
  interconnect_pons (pon1, pon2, m_a.r, gen);
  
  // Generate traffic.
  generate_traffic (pon1, m_a, gen, m_uv);

  // Calculate the mean ONU performance.
  double perfor = m_uv + m_seed;

  // Report the result back to the simulation object.
  m_sim.report (m_uv, m_seed, perfor);
}
