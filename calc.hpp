#include "args.hpp"
#include "graph.hpp"
#include "simulation.hpp"

// An object of this class calculates the performance.
class calc
{
  // Reference to the simulation object.
  simulation &m_sim;

  // The graph we operate on.
  const graph &m_pon;

  // Utilization value.
  double m_uv;

public:
  // Takes the arguments needed for a calculation.
  calc(simulation &sim, const graph &pon, double uv);
  
  // Does the calculation.
  void operator ()();
};
