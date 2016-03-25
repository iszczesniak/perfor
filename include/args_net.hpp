#ifndef ARGS_NET_HPP
#define ARGS_NET_HPP

#include <tuple>

/**
 * These are the network arguments.  They uniquely identify the network.
 */
template<typename T>
struct args_net
{
  /// Splitting ratio.
  int sratio;

  /// The probability that a fiber from a RN leads to the next stage.
  T s;

  /// The number of stages of the PON.
  int stages;

  /// The down rate in bps.
  T drate;

  /// The up rate in bps.
  T urate;

  /// The probability that a remote node is active.
  T q;

  /// The probability that an ONU is connected to the other operator.
  T r;

  /// The seed value.
  int seed;
  
  // The comparison operator.
  bool operator < (const args_net &args) const
  {
    auto as_tuple = [](const args_net &a)
      {
        return std::tie(a.sratio, a.s, a.stages, a.drate, a.urate,
                        a.r, a.q, a.seed);
      };

    return as_tuple(*this) < as_tuple(args);
  }
};

#endif /* ARGS_NET_HPP */
