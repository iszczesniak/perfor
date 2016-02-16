#include "progress.hpp"

#include <iostream>

progress::progress(int target): m_target (target), m_count (), 
                                t (std::cerr, 2, "%w s elapsed\n")
{
}

void
progress::report()
{
  m_mutex.lock ();
  ++m_count;
  std::cerr << "Progress " << 100.0 * m_count / m_target << " %, ";
  t.report();
  m_mutex.unlock ();
}
