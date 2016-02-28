#include "progress.hpp"

#include <iostream>
#include <string>

using namespace std;

progress::progress(int target): m_target (target), m_count (), 
                                t (cerr, 2, "%w s elapsed\n")
{
}

void
progress::report(const string &text)
{
  m_mutex.lock ();
  ++m_count;
  cerr << "Progress " << 100.0 * m_count / m_target << " %, ";
  t.report();
  cerr << text;
  m_mutex.unlock ();
}
