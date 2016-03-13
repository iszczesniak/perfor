#include "args.hpp"
#include "graph.hpp"
#include "simulation.hpp"

// An object of this class calculates the performance.
class calc
{
  // Reference to the simulation object.
  simulation &m_sim;

  // The graph we operate on.
  graph &m_pon;

  // Seed for this calculation.
  int m_seed;

  // Utilization value.
  double m_uv;

public:
  // Takes the arguments needed for a calculation.
  calc(simulation &sim, const graph &pon, int seed, double uv);
  
  // Does the calculation.
  void operator ()();
};
