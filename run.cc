#include "run.hpp"

#include "broker.hpp"
#include "graph.hpp"
#include "perfor.hpp"
#include "results.hpp"
#include "simulation.hpp"
#include "utils_tragen.hpp"

using namespace std;

run::run(const sim &sim, const args_run &args):
  m_sim (sim), m_args (args)
{
}

void
run::operator()()
{
  // Get the network we operate on.
  const graph &g = net_factory(args.net);

  // The traffic requested.
  auto req = generate_traffic (g, m_args.uv);

  // The traffic allocated.
  auto all = broker <graph> (g).service (req);

  // The results object.
  results r;

  // Calculate the mean ONU performance.
  r.mean_perf = calc_mean_perf (g, req, all);

  // Calculate the mean connectivity.
  r.mean_conn = calc_mean_conn (g);

  // Report the result back to the simulation object.
  m_sim.report (m_args, r);
}
