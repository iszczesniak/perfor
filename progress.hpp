#ifndef PROGRESS_HPP
#define PROGRESS_HPP

#include <boost/timer/timer.hpp>

#include <mutex>

class progress
{
  int m_target;
  int m_count;
  std::mutex m_mutex;
  boost::timer::auto_cpu_timer t;

public:
  progress (int target);
  void
  report ();
};

#endif /* PROGRESS_HPP */
