#include "args_run.hpp"
#include "graph.hpp"
#include "net_fac.hpp"
#include "sim.hpp"
#include "utils_tragen.hpp"

// An object of this class calculates the performance.
template<typename G>
class run
{
  // The graph we operate on.
  const G &m_g;

  // Reference to the simulation object.
  sim &m_sim;

  // The arguments of the run.
  args_run &m_args;
  
public:
  // Takes the arguments needed for a run.
  run(const sim &sim, const args_run &args):
    m_sim (sim), m_args (args)
  {
  }

  // Does the calculation.
  void operator ()()
  {
    // Get the network we operate on.
    const G &g = net_factory::get(args.net);

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
};
