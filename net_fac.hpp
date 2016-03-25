#ifndef NET_FAC_HPP
#define NET_FAC_HPP

#include <utility>

// The network factory.
template<typename G>
struct net_fac
{
  typedef typename std::map<args_net<double>, G> a2n_type;
  static a2n_type a2n;

  static const G &
  get (const args_net<double> &args)
  {
    auto i = a2n.find(args);
    if (i == a2n.end())
      {
        // Generate the graph, because it was not generated before.
        i = a2n.insert(std::make_pair(args, G())).first;
        generate_pon (i->second, args);
      }
    return i->second;
  }
};

#endif /* NET_FAC_HPP */
