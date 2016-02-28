#ifndef PROGRESS_HPP
#define PROGRESS_HPP

#include <boost/timer/timer.hpp>

#include <mutex>
#include <string>

class progress
{
  int m_target;
  int m_count;
  std::mutex m_mutex;
  boost::timer::auto_cpu_timer t;

public:
  progress (int target);
  void
  report (const std::string &text = "");
};

#endif /* PROGRESS_HPP */
