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

calc::calc(simulation &sim, const graph &pon, int seed, double uv):
  m_sim (sim), m_pon (pon), m_seed (seed), m_uv (uv)
{
}

void
calc::operator()()
{
  // Random number generator.
  mt19937 gen (m_seed);

  // The traffic requested.
  auto req = generate_traffic (pon, m_a, gen, m_uv);

  // The traffic allocated.
  auto all = allocate (pon, req);

  // The results object.
  results r;

  // Calculate the mean ONU performance.
  r.mean_perf = calc_mean_perf (m_pon, req, all);

  // Calculate the mean connectivity.
  r.mean_conn = calc_mean_conn (pon);

  // Report the result back to the simulation object.
  m_sim.report (m_uv, m_seed, r);
}
