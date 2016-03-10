#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "args.hpp"
#include "progress.hpp"
#include "results.hpp"

#include <map>
#include <mutex>
#include <string>
#include <utility>

class simulation
{
  // Simulation arguments.
  args m_a;

  // Result type for different seeds.
  typedef std::map <int, double> rfds_type;

  // Result type for dirretent utilization values.
  typedef std::map <double, rfds_type> results_type;

  // Here the threads store their solutions.
  results_type m_results;
  std::mutex m_results_mutex;

  // Progress indicator;
  progress m_pi;
  
public:
  simulation (const args &a);
  void run ();
  void report (double uv, int seed, results &r);
  void print ();
};

#endif /* SIMULATION_HPP */
