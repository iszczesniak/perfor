#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "args.hpp"
#include "graph.hpp"
#include "progress.hpp"
#include "results.hpp"

#include <map>
#include <mutex>
#include <string>
#include <utility>

class simulation
{
  // Result type for different seeds.
  typedef std::map <int, double> rfds_type;

  // Result type for different utilization values.
  typedef std::map <double, rfds_type> results_type;

  // The type of the container with the simulation graphs.
  typedef std::map <int, graph> I2G;

  // Simulation arguments.
  args m_a;

  // Here the results are stored.
  results_type m_results;
  std::mutex m_results_mutex;

  // Progress indicator;
  progress m_pi;

  // The simulation graphs.
  I2G i2g;
  
public:
  simulation (const args &a);
  void run ();
  void report (double uv, int id, results &r);
  void print ();
};

#endif /* SIMULATION_HPP */
