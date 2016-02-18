#include "simulation.hpp"

#include "graph.hpp"
#include "progress.hpp"
#include "utils.hpp"
#include "utils_netgen.hpp"

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/error_of.hpp>
#include <boost/accumulators/statistics/error_of_mean.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/thread/thread.hpp>

#include <cmath>
#include <list>
#include <mutex>
#include <random>
#include <thread>

using namespace std;
namespace ba = boost::accumulators;
namespace as = boost::asio;

// An object of this class calculates the performance.
class calc
{
  // Arguments of the simulation.
  args m_a;

  // Reference to the simulation object.
  simulation &m_sim;

  // Seed for this calculation.
  int m_seed;

public:
  calc(simulation &sim, const args &a, int seed, double uv):
    m_sim (sim), m_a (a), m_seed(seed), m_pb (pb)
  {
  }

  void operator()()
  {
    // Random number generator.
    mt19937 gen (m_seed);

    // Generate PON.
    graph pon;
    generate_pon (pon, m_a, gen);

    // Generate traffic.
    generate_traffic(pon, gen);

    // Calculate the mean ONU performance.
    double perfor = calc_mean_perfor (pon);

    // Report the result back to the simulation object.
    m_sim.report (m_uv, m_seed, perfor);
  }
};

// This function runs the simulations, multithreading them.
void
run (args a, const std::list <dblp> &rqs, int seeds)
{
  as::io_service ios;
  auto_ptr <as::io_service::work>
    work (new as::io_service::work (ios));
  boost::thread_group threads;

  // The number of threads supported by hardware.
  int noth = boost::thread::hardware_concurrency (); 
  for (int i = 0; i < noth; ++i)
    threads.create_thread (boost::bind (&as::io_service::run, &ios));

  progress pb (seeds * rqs.size ());

  for (int seed = 1; seed <= seeds; ++seed)
    {
      a.seed = seed;

      for (auto &rq: rqs)
        {
          a.r = rq.first;
          a.q = rq.second;
          ios.post (calc (a, pb));
        }
    }

  // This is needed here, so that all tasks finish.
  work.reset ();
  threads.join_all ();
  ios.stop ();
}

class results
{
  // Here the threads store their solutions.
  results m;
  std::mutex m_mutex;
  

  m_mutex.lock ();
    m[m_a.r][m_a.q].push_back (availa);
    m_mutex.unlock ();
  
    for (const auto &rp: m)
    {
      for (const auto &qp: rp.second)
        {
          typedef ba::tag::error_of <ba::tag::mean> eom_t;
          typedef ba::stats <ba::tag::mean, eom_t> stats_t;
          ba::accumulator_set <double, stats_t> acc;

          for (const auto &l: qp.second)
            acc (l);

          // The mean.
          double mean = ba::mean (acc);
          // Standard error of the mean.
          double se = ba::error_of <ba::tag::mean> (acc);
          // Relative standard error of the mean in percent.
          double rse = 100 * se / mean;

          if (rse > 1)
            cerr << "PROBLEM: rse = " << rse << endl;

          cout << rp.first << " "
               << qp.first << " "
               << mean << " "
               << rse << endl;
        }

      cout << endl;
    }
};
