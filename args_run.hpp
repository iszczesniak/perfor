#ifndef ARGS_RUN_HPP
#define ARGS_RUN_HPP

#include "args_net.hpp"

/**
 * These are the simulation arguments for all runs, not for a specific
 * simulation run.
 */
struct args_run
{
  // The network arguments.
  args_net net;

  // The utilization value;
  double uv;
};

#endif /* ARGS_RUN_HPP */
