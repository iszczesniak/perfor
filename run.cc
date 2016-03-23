#include "calc.hpp"

#include "broker.hpp"
#include "graph.hpp"
#include "perfor.hpp"
#include "results.hpp"
#include "simulation.hpp"
#include "utils_tragen.hpp"

using namespace std;

calc::calc(simulation &sim, const graph &g, double uv, int id):
  m_sim (sim), m_g (g), m_uv (uv), m_id (id)
{
}

void
calc::operator()()
{
  // The traffic requested.
  auto req = generate_traffic (m_g, m_uv);

  // The traffic allocated.
  auto all = broker <graph> (m_g).service (req);

  // The results object.
  results r;

  // Calculate the mean ONU performance.
  r.mean_perf = calc_mean_perf (m_g, req, all);

  // Calculate the mean connectivity.
  r.mean_conn = calc_mean_conn (m_g);

  // Report the result back to the simulation object.
  m_sim.report (m_uv, m_id, r);
}
