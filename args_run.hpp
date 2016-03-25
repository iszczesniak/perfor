#ifndef ARGS_RUN_HPP
#define ARGS_RUN_HPP

#include "args_net.hpp"

#include <tuple>

/**
 * These are the simulation arguments for all runs, not for a specific
 * simulation run.
 */
template<typename T>
struct args_run
{
  // The network arguments.
  args_net<T> net;

  // The utilization value;
  T uv;

  // The comparison operator.
  bool operator < (const args_run &args) const
  {
    return std::tie(net, uv) < std::tie(args.net, args.uv);
  }
};

#endif /* ARGS_RUN_HPP */
