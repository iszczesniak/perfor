#ifndef ARGS_SIM_HPP
#define ARGS_SIM_HPP

#include <vector>

/**
 * These are the simulation arguments for all runs, not for a specific
 * simulation run.
 */
struct args_sim
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
  std::vector<double> qs;

  /// The probability that an ONU is connected to the other operator.
  std::vector<double> rs;

  // The utilization values;
  std::vector<double> uvs;

  /// The number of seeds.
  int seeds;
};

#endif /* ARGS_SIM_HPP */
