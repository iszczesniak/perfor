#include "calc.hpp"

#include "graph.hpp"
#include "perfor.hpp"
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
  name_vertices(pon);

  // Generate traffic.
  generate_traffic (pon, m_a, gen, m_uv);

  // Allocate the traffic.
  allocate (pon);
  
  // Calculate the mean ONU performance.
  double perfor = calc_mean_perfor (pon);

  // ostringstream o;
  // print_pon (pon, o);
  // o << "------------------------------------------" << endl;
  // string text = o.str ();

  // Report the result back to the simulation object.
  m_sim.report (m_uv, m_seed, perfor);
}
