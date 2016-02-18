#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <map>
#include <mutex>
#include <utility>

class simulation
{
  // Double pair.
  typedef std::pair <double, double> double_pair;

  // The simulation input.
  typedef std::pair <double_pair, int> sim_input;

  // Result type of a single simulation run.
  typedef std::pair <sim_input, double_pair> results;

  // Here the threads store their solutions.
  results m;
  std::mutex m_mutex;

public:
  simulation(const args &a);
  void run();
  void print();
};

#endif /* SIMULATION_HPP */
