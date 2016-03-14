#include "simulation.hpp"

#include "calc.hpp"
#include "progress.hpp"
#include "utils_netgen.hpp"

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/error_of_mean.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/thread/thread.hpp>

#include <list>
#include <mutex>
#include <random>
#include <thread>

using namespace std;
namespace ba = boost::accumulators;
namespace as = boost::asio;

simulation::simulation(const args &a):
  m_a(a), m_pi(m_a.seeds * m_a.uvs.size())
{
}

void
simulation::run ()
{
  assert(m_a.seeds >= 1);

  as::io_service ios;
  auto_ptr <as::io_service::work> work (new as::io_service::work (ios));
  boost::thread_group threads;

  // The number of threads supported by hardware.
  int noth = boost::thread::hardware_concurrency ();
  for (int i = 0; i < noth; ++i)
    threads.create_thread (boost::bind (&as::io_service::run, &ios));

  // Generate networks and queue the tasks.
  for (int seed = 1; seed <= m_a.seeds; ++seed)
    {
      std::mt19937 gen;
      auto &pon = s2d[seed];
      generate_pon (pon, m_a, gen);
      for (double uv: m_a.uvs)
        ios.post (calc (*this, pon, seed, uv));
    }
  
  // This is needed here, so that all tasks finish.
  work.reset ();
  threads.join_all ();
  ios.stop ();
}

void
simulation::report(double uv, int id, results &r)
{
  m_results_mutex.lock ();
  m_results[uv][id] = r.mean_perf;
  m_results_mutex.unlock ();

  // Update the progress indicator.
  m_pi.report (r.text);
}

void
simulation::print()
{
  // Iterate over the results.  Don't modify the results to make sure
  // we got the results of all tasks.
  const results_type &results = m_results;
  
  for (double uv: m_a.uvs)
      {
        results_type::const_iterator i = results.find (uv);
        assert (i != results.end());
        const rfds_type &rfds = i->second;

        typedef ba::tag::error_of <ba::tag::mean> eom_t;
        typedef ba::stats <ba::tag::mean, eom_t> stats_t;
        ba::accumulator_set <double, stats_t> acc;

        for (int seed = 1; seed <= m_a.seeds; ++seed)
          {
            rfds_type::const_iterator j = rfds.find (seed);
            assert (j != rfds.end ());
            acc(j->second);
          }

        // The mean.
        double mean = ba::mean (acc);
        // Standard error of the mean.
        double se = ba::error_of <ba::tag::mean> (acc);
        // Relative standard error of the mean in percent.
        double rse = 100 * se / mean;

        cout << uv << " "
             << mean << " "
             << rse << endl;
      }
}
