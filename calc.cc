#include "calc.hpp"

#include "graph.hpp"
#include "simulation.hpp"

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
  graph pon;
  //  generate_pon (pon, m_a, gen);

  // Generate traffic.
  //  generate_traffic(pon, gen);

  // Calculate the mean ONU performance.
  double perfor = 1; // calc_mean_perfor (pon);

  // Report the result back to the simulation object.
  m_sim.report (m_uv, m_seed, perfor);
}
