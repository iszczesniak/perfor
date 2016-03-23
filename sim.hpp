#ifndef SIM_HPP
#define SIM_HPP

#include "args_run.hpp"
#include "args_sim.hpp"
#include "graph.hpp"
#include "progress.hpp"
#include "results.hpp"

#include <map>
#include <mutex>
#include <string>
#include <utility>

class sim
{
  // Simulation arguments.
  args_sim m_a;

  // The type that maps the arguments to resutls.
  typedef std::map <args_run, results> a2r_type;

  // Here the results are stored.
  a2r_type m_a2r;
  std::mutex m_a2r_mutex;

  // Progress indicator;
  progress m_pi;

public:
  sim (const args_sim &a);
  void run ();
  void report (const args_run &args, const results &r);
  void print (std::ostream &);
};

#endif /* SIM_HPP */
