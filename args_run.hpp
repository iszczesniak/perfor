#ifndef ARGS_RUN_HPP
#define ARGS_RUN_HPP

#include <vector>

/**
 * These are the simulation arguments for all runs, not for a specific
 * simulation run.
 */
struct args_run
{
  /// Splitting ratio.
  int sratio;

  /// The probability that a fiber from a RN leads to the next stage.
  double s;

  /// The number of stages of the PON.
  int stages;

  /// The down rate in bps.
  double drate;

  /// The up rate in bps.
  double urate;

    /// The probability that a remote node is active.
  double q;

  /// The probability that an ONU is connected to the other operator.
  double rs;

  // The utilization value;
  double uv;

  /// The seed value.
  int seed;
};

#endif /* ARGS_RUN_HPP */
