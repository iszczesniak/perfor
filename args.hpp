#ifndef ARGS_HPP
#define ARGS_HPP

#include <string>

using namespace std;

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

  /// The seed.
  int seed;

  /// The number of stages of the PON.
  int stages;
};

#endif /* ARGS_HPP */
