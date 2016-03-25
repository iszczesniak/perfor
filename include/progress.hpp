#ifndef PROGRESS_HPP
#define PROGRESS_HPP

#include <boost/timer/timer.hpp>

#include <mutex>
#include <string>
#include <ostream>

template<typename T>
class progress
{
  std::ostream &m_out;
  T m_target;
  T m_count;
  std::mutex m_mutex;
  boost::timer::auto_cpu_timer t;

public:
  progress(std::ostream &out): m_out(out), m_target (), m_count (), 
                               t (m_out, 2, "%w s elapsed\n")
  {
  }

  void
  set (T count)
  {
    m_count = count;
  }

  void
  report (const std::string &text = "")
  {
    m_mutex.lock ();
    ++m_count;
    m_out << "Progress " << 100.0 * m_count / m_target << " %, ";
    t.report();
    m_out << text;
    m_mutex.unlock ();
  }
};

#endif /* PROGRESS_HPP */
