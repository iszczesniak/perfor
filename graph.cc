#include "graph.hpp"

using namespace std;

string
to_string (VERTEX_T t)
{
  switch (t)
    {
    case VERTEX_T::ONU:
      return "ONU";
    case VERTEX_T::ICO:
      return "ICO";
    case VERTEX_T::ARN:
      return "ARN";
    case VERTEX_T::PRN:
      return "PRN";
    case VERTEX_T::OLT:
      return "OLT";
    }

  abort();
}
