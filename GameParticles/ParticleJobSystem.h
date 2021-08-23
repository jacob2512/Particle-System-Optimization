#pragma once
#include <thread>
#include <synchapi.h>

class ParticleJobSystem
{
public:
  ParticleJobSystem();
  ~ParticleJobSystem();

  void KickUpdateJobs();
  void Split();

  static void threadentry();

private:
  static constexpr int max_threads = 4;

  std::thread particle_thread_array[max_threads];

  int first_unclaimed_entry;
  int complete_entries;
  int total_entries;

  CRITICAL_SECTION thread_entry_guard;
  CONDITION_VARIABLE thread_entry_wait_state;
  CONDITION_VARIABLE thread_entry_stop_state;
};