#ifndef ARGS_HPP
#define ARGS_HPP

#include <vector>

/**
 * These are the simulation arguments.
 */
struct args
{
  /// Splitting ratio.
  int sratio;

  /// The probability that a remote node is active.
  double q;

  /// The probability that an ONU is connected to the other operator.
  double r;

  /// The probability that a fiber from a RN leads to the next stage.
  double s;

  /// The number of stages of the PON.
  int stages;

  /// The down rate in bps.
  double drate;

  /// The up rate in bps.
  double urate;

  // The utilization values;
  std::vector<double> uvs;

  /// The number of seeds.
  int seeds;
};

#endif /* ARGS_HPP */
