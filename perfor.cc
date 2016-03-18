#include "args.hpp"
#include "simulation.hpp"

#include <iostream>

using namespace std;

int
main (int argc, const char* argv[])
{
  // Arguments that we pass around.
  args a;

  // The splitting ratio.
  a.sratio = 32;

  // The probability that a remote node is active.
  a.q = 0.1;

  // The probability that an ONU is connected to the other operator.
  a.r = 0.05;

  // The splitting probability.
  a.s = 0.3;

  // Integer parameters.
  a.stages = 3;

  // The down rate in bps.
  a.drate = 10 * 1e9;

  // The up rate in bps.
  a.urate = 2.5 * 1e9;

  // The number of seeds, i.e., simulation runs for the given
  // parameters.
  a.seeds = 1;
  
  a.uvs = {0.1};

  // {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3,
  //  1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0};

  simulation sim(a);
  sim.run();
  sim.print(cout);

  return 0;
}
