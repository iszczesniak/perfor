#include "args.hpp"
#include "simulation.hpp"

// An object of this class calculates the performance.
class calc
{
  // Reference to the simulation object.
  simulation &m_sim;

  // Arguments of the simulation.
  args m_a;

  // Seed for this calculation.
  int m_seed;

  // Utilization value.
  double m_uv;

public:
  // Takes the arguments needed for a calculation.
  calc (simulation &sim, const args &a, int seed, double uv);

  // Does the calculation.
  void operator ()();
};
