#ifndef ARGS_RUN_HPP
#define ARGS_RUN_HPP

#include "args_net.hpp"
#include "args_sim.hpp"

#include <tuple>

/**
 * These are the simulation arguments for all runs, not for a specific
 * simulation run.
 */
template<typename T>
struct args_run
{
  // The network arguments.
  args_net<T> m_net;

  // The utilization value;
  T m_uv;

  args_run(const args_sim &args, T q, T r, T uv, int seed)
  {
    // The custom arguments.
    m_net.q = q;
    m_net.r = r;
    m_uv = uv;
    m_net.seed = seed;

    // The simulation arguments.
    m_net.s = args.s;
    m_net.sratio = args.sratio;
    m_net.stages = args.stages;
    m_net.drate = args.drate;
    m_net.urate = args.urate;
  }

  // The comparison operator.
  bool operator < (const args_run &args) const
  {
    return std::tie(m_net, m_uv) < std::tie(args.m_net, args.m_uv);
  }
};

template<typename S, typename T>
S &
operator << (S &out, const args_run<T> &args)
{
  out << args.m_net;
  out << "m_uv = " << args.m_uv << '\n';
  return out;
}

#endif /* ARGS_RUN_HPP */
