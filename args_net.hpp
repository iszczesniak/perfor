#ifndef ARGS_NET_HPP
#define ARGS_NET_HPP

/**
 * These are the network arguments.  They uniquely identify the network.
 */
struct args_net
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
  double r;

  /// The seed value.
  int seed;
};

#endif /* ARGS_NET_HPP */
