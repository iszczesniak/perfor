#include "sim.hpp"

#include "run.hpp"
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

sim::sim(const args_sim &args):
  m_args(args)
{
  make_runs();
}

void
sim::run ()
{
  assert(m_args.seeds >= 1);

  // Set the progress iterator target value.
  pi.set(m_runs.size());

  as::io_service ios;
  auto_ptr <as::io_service::work> work (new as::io_service::work (ios));
  boost::thread_group threads;

  // The number of threads supported by hardware.
  int noth = boost::thread::hardware_concurrency ();
  for (int i = 0; i < noth; ++i)
    threads.create_thread (boost::bind (&as::io_service::run, &ios));

  // Run the runs.
  for (const args_run &a: m_runs)
    ios.post (run (*this, a));

  // This is needed here, so that all tasks finish.
  work.reset ();
  threads.join_all ();
  ios.stop ();
}

void
simulation::report(const args_run &args, const results &r)
{
  m_a2r_mutex.lock ();
  m_a2r[args] = r;
  m_a2r_mutex.unlock ();

  // Update the progress indicator.
  m_pi.report (r.text);
}

void
simulation::print(ostream &out)
{
  for (auto uv: m_args.uvs)
      {
        results_type::const_iterator i = m_results.find (uv);
        assert (i != m_results.end());
        // Here we have the results for the uv, all of them, for all
        // seed values.
        const rfds_type &rfds = i->second;

        typedef ba::tag::error_of <ba::tag::mean> eom_t;
        typedef ba::stats <ba::tag::mean, eom_t> stats_t;
        ba::accumulator_set <double, stats_t> perfor_acc;

        // Iterate over the seed values.
        for (int seed = 1; seed <= m_args.seeds; ++seed)
          {
            rfds_type::const_iterator j = rfds.find (seed);
            assert (j != rfds.end ());
            perfor_acc(j->second);
          }

        // The mean of the performance.
        double perfor_mean = ba::mean (perfor_acc);
        // Standard error of the mean of the performance.
        double perfor_se = ba::error_of <ba::tag::mean> (perfor_acc);
        // Relative standard error of the mean in percent of performance.
        double perfor_rse = 100 * perfor_se / perfor_mean;

        out << uv << " "
            << perfor_mean << " "
            << perfor_rse << endl;
      }
}

void
sim::make_runs()
{
  args_run args;
  args.net.sratio = m_args.sratio;
  args.net.s = m_args.s;
  args.net.stages = m_args.stages;
  args.net.drate = m_args.drate;
  args.net.urate = m_args.urate;

  for(auto uv: m_args.uvs)
    for(auto r: m_args.rs)
      for(auto q: m_args.qs)
        {
          runs.push_back(args);
          args.uv = uv;
          args.net.r = r;
          args.net.q = q;
          args_run(m_args, uv, r, q);
        }
}
