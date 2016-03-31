#ifndef SIM_HPP
#define SIM_HPP

#include "args_run.hpp"
#include "args_sim.hpp"
#include "progress.hpp"
#include "results.hpp"
#include "run.hpp"
#include "utils_netgen.hpp"

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/error_of_mean.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/thread/thread.hpp>

#include <deque>
#include <map>
#include <mutex>
#include <ostream>

namespace ba = boost::accumulators;
namespace as = boost::asio;

template<typename G>
class sim
{
  // Simulation arguments.
  args_sim m_args;

  // The type that maps the arguments to resutls.
  typedef std::map <args_run<double>, results> a2r_type;

  // Here the results are stored.
  a2r_type m_a2r;
  std::mutex m_a2r_mutex;

  // Progress indicator;
  progress<int> m_pi;

  // The arguments for runs.
  std::deque<args_run<double>> m_runs;

public:
  sim (const args_sim &args, std::ostream &out):
    m_args(args), m_pi(out)
  {
    make_runs();
    run_runs();
    print(out);
  }

  void
  report (const args_run<double> &args, const results &r)
  {
    m_a2r_mutex.lock ();
    m_a2r[args] = r;
    m_a2r_mutex.unlock ();

    // Update the progress indicator.
    m_pi.report (r.text);
  }

private:
  void
  make_runs()
  {
    for(auto q: m_args.qs)
      for(auto r: m_args.rs)
        for(auto uv: m_args.uvs)
          for(int seed = 1; seed <= m_args.seeds; ++seed)
            m_runs.push_back(args_run<double>(m_args, q, r, uv, seed));
  }

  void
  run_runs()
  {
    assert(m_args.seeds >= 1);

    // Set the progress iterator target value.
    m_pi.set(m_runs.size());

    as::io_service ios;
    std::auto_ptr <as::io_service::work> work (new as::io_service::work (ios));
    boost::thread_group threads;

    // The number of threads supported by hardware.
    int noth = boost::thread::hardware_concurrency ();
    for (int i = 0; i < noth; ++i)
      threads.create_thread (boost::bind (&as::io_service::run, &ios));

    //Run the runs.
    for (const args_run<double> &a: m_runs)
      ios.post (run<G> (*this, a));

    // This is needed here, so that all tasks finish.
    work.reset ();
    threads.join_all ();
    ios.stop ();
  }

  void
  print (std::ostream &out)
  {
    for(auto q: m_args.qs)
      for(auto r: m_args.rs)
        for(auto uv: m_args.uvs)
          {
            typedef ba::tag::error_of <ba::tag::mean> eom_t;
            typedef ba::stats <ba::tag::mean, eom_t> stats_t;
            ba::accumulator_set <double, stats_t> perfor_acc;

            // Iterate over the seed values.
            for (int seed = 1; seed <= m_args.seeds; ++seed)
              {
                args_run<double> args(m_args, q, r, uv, seed);
                const results &r = m_a2r.at(args);
                perfor_acc(r.mean_perf);
              }

            // The mean of the performance.
            double perfor_mean = ba::mean (perfor_acc);
            // Standard error of the mean of the performance.
            double perfor_se = ba::error_of <ba::tag::mean> (perfor_acc);
            // Relative standard error of the mean in percent of performance.
            double perfor_rse = 100 * perfor_se / perfor_mean;

            out << uv << " "
                << perfor_mean << " "
                << perfor_rse << std::endl;
          }
  }
};

#endif /* SIM_HPP */
