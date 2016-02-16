#include "args.hpp"

#include <iostream>

#include <boost/program_options.hpp>

using namespace std;
namespace po = boost::program_options;

args
process_args (int argc, const char *argv[])
{
  args result;

  try
    {
      po::options_description opts ("The options");

      opts.add_options ()
        ("help,h", "produce help message")

        ("seed,s", po::value <int> ()->default_value (1),
         "the seed of the random number generator");

      po::variables_map vm;
      po::store (po::command_line_parser (argc, argv).options
                 (opts).run (), vm);

      if (vm.count("help"))
        {
          cout << opts << "\n";
          exit(0);
        }

      // If there is something wrong with parameters, we will get
      // notified automatically and the program will exit.
      po::notify(vm);

      result.seed = vm["seed"].as <int> ();
    }
  catch (const std::exception& e)
    {
      cerr << e.what () << std::endl;
      exit(1);
    }

  return result;
}
