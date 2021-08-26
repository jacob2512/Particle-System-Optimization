#pragma once
#include <thread>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "ParticleRingBuffer.h"

class ParticleJobSystem
{
public:
  ParticleJobSystem();
  ~ParticleJobSystem();

  void SyncUpdateJobs();
  void KickUpdateJobs(ParticleRingBuffer* particle_array, float dt);

  static void thread_entry();

private:
  static constexpr int max_threads = 4;
  int first_unclaimed_entry;
  int complete_entries;
  int total_entries;

  bool should_stop;

  ParticleRingBuffer* stored_particle_array;
  float stored_dt;

  CRITICAL_SECTION cs_guard;
  CONDITION_VARIABLE cv_wait_for_jobs;
  CONDITION_VARIABLE cv_jobs_complete;

  std::thread particle_thread_array[max_threads];
};

extern ParticleJobSystem particle_job_system;