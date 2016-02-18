int
main (int argc, const char* argv[])
{
  // Arguments that we pass around.
  args a;

  // The splitting probability.
  a.s = 0.3;

  // Integer parameters.
  a.stages = 3;

  // The splitting ratio.
  a.sratio = 32;

  // The number of seeds, i.e., simulation runs for the given
  // parameters.
  a.seeds = 100;
  
  a.uvs = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0,
           1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0};

  simulation sim(a);
  sim.run();
  sim.print();

  return 0;
}
