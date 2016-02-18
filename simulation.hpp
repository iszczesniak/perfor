#ifndef SIMULATION_HPP
#define SIMULATION_HPP

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

class simulation
{
  // Double pair.
  typedef pair <double, double> double_pair;

  // The simulation input.
  typedef pair <double_pair, int> sim_input;

  // Result type of a single simulation run.
  typedef map <sim_input, double_pair> results;

  // Here the threads store their solutions.
  results m;
  std::mutex m_mutex;

public:
  simulation(const args &a);
  void run();
  void print();
};

#endif SIMULATION_HPP
