#ifndef RUN_HPP
#define RUN_HPP

#include "args_run.hpp"
#include "broker.hpp"
#include "graph.hpp"
#include "perfor.hpp"
#include "utils_netgen.hpp"
#include "utils_tragen.hpp"

template<typename G>
class sim;

// An object of this class calculates the performance.
template<typename G>
class run
{
  // Reference to the simulation object.
  sim<G> &m_sim;

  // The arguments of the run.
  const args_run<double> &m_args;
  
public:
  // Takes the arguments needed for a run.
  run(sim<G> &sim, const args_run<double> &args):
    m_sim (sim), m_args (args)
  {
  }

  // Does the calculation.
  void operator ()()
  {
    // Get the network we operate on.
    G g = generate_pon<G>(m_args.m_net);

    // The traffic requested.
    auto req = generate_traffic (g, m_args.m_uv);

    // The traffic allocated.
    auto all = broker <G> (g).service (req);

    // The results object.
    results r;

    // Calculate the mean ONU performance.
    r.mean_perf = calc_mean_perf<G> (g, req, all);

    // Calculate the mean connectivity.
    r.mean_conn = calc_mean_conn<G> (g);

    // Report the result back to the simulation object.
    m_sim.report (m_args, r);
  }
};

#endif /* RUN_HPP */
