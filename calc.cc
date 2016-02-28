#include "calc.hpp"

#include "graph.hpp"
#include "simulation.hpp"
#include "utils_netgen.hpp"
#include "utils_tragen.hpp"

#include <random>
#include <sstream>

using namespace std;

calc::calc(simulation &sim, const args &a, int seed, double uv):
  m_sim (sim), m_a (a), m_seed (seed), m_uv (uv)
{
}

void
calc::operator()()
{
  // Random number generator.
  mt19937 gen (m_seed);

  // Generate PON.
  graph pon;
  generate_pon (pon, m_a, gen);
  
  // Generate traffic.
  generate_traffic (pon, m_a, gen, m_uv);

  // Calculate the mean ONU performance.
  double perfor = m_uv + m_seed;

  ostringstream o;
  print_pon (pon, cout);
  string text = o.str ();

  // Report the result back to the simulation object.
  m_sim.report (m_uv, m_seed, perfor);
}
