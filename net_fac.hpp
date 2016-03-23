#ifndef NET_FAC_HPP
#define NET_FAC_HPP

#include <random>
#include <utility>

// The network factory.
template<typename G>
struct net_fac
{
  typedef typename std::map<args_net, G> a2n_type;
  static a2n_type a2n;

  get (const args_net<G> &args)
  {
    auto i = a2n.find(args);
    if (i == a2n.end())
      {
        i = insert(std::make_pair(args, G()));
        std::mt19937 gen;
        gen.seed(args.seed);
        generate_pon (i->second, m_a, gen);
      }

    return i->second;
  }
};

#endif /* NET_FAC_HPP */
