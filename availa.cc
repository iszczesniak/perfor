#include "availa.hpp"
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

typedef pair <double, double> dblp;
typedef map <double, map <double, list <double>>> results;

// Here the threads store their solutions.
results m;
std::mutex m_mutex;

class calc
{
  args m_a;
  progress &m_pb;

public:
  calc(const args &a, progress &pb):
    m_a (a), m_pb (pb)
  {
  }

  void operator()()
  {
    // Random number generator.
    mt19937 gen (m_a.seed);

    graph g;
    generate_pon (g, m_a, gen);

    // Calculate the mean ONU availability.
    double availa = calc_mean_availa (g);

    m_mutex.lock ();
    m[m_a.r][m_a.q].push_back (availa);
    m_mutex.unlock ();

    m_pb.report ();
  }
};

void
run (args a, const std::list <dblp> &rqs, double seeds)
{
  as::io_service ios;
  auto_ptr <as::io_service::work>
    work (new as::io_service::work (ios));
  boost::thread_group threads;

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

int
main (int argc, const char* argv[])
{
  args a = process_args (argc, argv);

  // Fiber availability per km.
  const double fapkm = 0.9999429;

  // The availability of an OLT;
  a.olta = 0.9999485;
  // The availability of an ONU;
  a.onua = 0.9999645;
  // The availability of a passive RN, specifically of the 1:32 power
  // splitter.
  a.prna = 0.9999987;
  // The availability of an active RN, specifically of an OLT.
  a.arna = a.olta;
  a.ffa = pow(fapkm, 10);
  a.dfa = pow(fapkm, 3);
  a.lfa = pow(fapkm, 0.7);

  // The splitting probability.
  a.s = 0.3;

  // Integer parameters.
  a.stages = 3;
  a.sratio = 32;

  double rs[] = {0.00, 0.001, 0.002, 0.003, 0.004, 0.005, 0.006, 0.007, 0.008, 0.009,
                 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09,
                 0.1, 0.15,
                 0.2, 0.25,
                 0.3, 0.35,
                 0.4, 0.45,
                 0.5, 0.55,
                 0.6, 0.65,
                 0.7, 0.75,
                 0.8, 0.85,
                 0.9, 0.95,
                 1.0};

  double qs[] = {0.0, 0.05,
                 0.1, 0.15,
                 0.2, 0.25,
                 0.3, 0.35,
                 0.4, 0.45,
                 0.5, 0.55,
                 0.6, 0.65,
                 0.7, 0.75,
                 0.8, 0.85,
                 0.9, 0.95,
                 1.0};

  int seeds = 100;
  std::list <dblp> rqs;

  for (auto r: rs)
    for (auto q: qs)
      rqs.push_back (dblp (r, q));

  run(a, rqs, seeds);

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

  return 0;
}
