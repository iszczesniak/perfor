#ifndef RESULTS_HPP
#define RESULTS_HPP

#include <map>
#include <string>

/**
 * These are the simulation results of a single run.
 */
struct args
{
  // Histogram of ONU path possibilities.
  std::map<int, int> hist;

  // Some text we want to have printed.
  std::string text;
};

#endif /* RESULTS_HPP */
