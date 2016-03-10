#include "calc.hpp"

#include "graph.hpp"
#include "perfor.hpp"
#include "results.hpp"
#include "simulation.hpp"
#include "utils_netgen.hpp"
#include "utils_tragen.hpp"

#include <random>
#include <sstream>

using namespace std;

calc::calc(simulation &sim, const args &a, int seed, double uv):
  m_sim (sim), m_a (a), m_seed (seed), m_uv (uv)
{
}

void
calc::operator()()
{
  // Random number generator.
  mt19937 gen (m_seed);

  // Generate PON.
  graph pon = generate_pon (pon, m_a, gen);

  // The traffic requested.
  V2D <G> req = generate_traffic (pon, m_a, gen, m_uv);

  // The traffic allocated.
  V2D <G> all = allocate (pon, req);

  // The results object.
  results r;

  // Calculate the mean ONU performance.
  r.mean_perf = calc_mean_perf (pon);

  // Calculate the mean connectivity.
  r.mean_conn = calc_mean_conn (pon);

  // Report the result back to the simulation object.
  m_sim.report (m_uv, m_seed, r);
}
