#include "ParticleJobSystem.h"
#include <cassert>

ParticleJobSystem particle_job_system;

ParticleJobSystem::ParticleJobSystem():
  first_unclaimed_entry(0),
  complete_entries(0),
  total_entries(0),
  should_stop(false),
  stored_particle_array(nullptr),
  stored_dt(0.0f)
{
  InitializeCriticalSection(&cs_guard);

  InitializeConditionVariable(&cv_wait_for_jobs);
  InitializeConditionVariable(&cv_jobs_complete);

  for (int i = 0; i < max_threads; i++)
  {
    particle_thread_array[i] = std::thread(thread_entry);
    
    //generate thread description
    char thread_desc[256];
    snprintf(thread_desc,sizeof(thread_desc),"job thread %d",i);
    size_t str_len;
    wchar_t thread_desc_wide[sizeof(thread_desc)];

    mbstowcs_s(&str_len, thread_desc_wide,thread_desc, sizeof(thread_desc));
    SetThreadDescription(particle_thread_array[i].native_handle(), thread_desc_wide);
  }
}

ParticleJobSystem::~ParticleJobSystem()
{
  EnterCriticalSection(&particle_job_system.cs_guard);

  should_stop = true;
  WakeAllConditionVariable(&cv_wait_for_jobs);

  LeaveCriticalSection(&particle_job_system.cs_guard);

  for (int i = 0; i < max_threads; i++)
  {
    particle_thread_array[i].join();
  }

  DeleteCriticalSection(&cs_guard);
}

void ParticleJobSystem::thread_entry()
{
  EnterCriticalSection(&particle_job_system.cs_guard);

  while (!particle_job_system.should_stop)
  {
    if (particle_job_system.first_unclaimed_entry < particle_job_system.total_entries)
    {
      Particle* p = particle_job_system.stored_particle_array->GetAt(particle_job_system.first_unclaimed_entry);
      particle_job_system.first_unclaimed_entry++;
      float cached_dt = particle_job_system.stored_dt;

      LeaveCriticalSection(&particle_job_system.cs_guard);

      // call every particle and update its position 
      p->Update(cached_dt);

      EnterCriticalSection(&particle_job_system.cs_guard);

      particle_job_system.complete_entries++;

      if (particle_job_system.complete_entries >= particle_job_system.total_entries)
      {
        WakeConditionVariable(&particle_job_system.cv_jobs_complete);
      }
    }
    else
    {
      SleepConditionVariableCS(&particle_job_system.cv_wait_for_jobs, &particle_job_system.cs_guard, INFINITE);
    }
  }

  LeaveCriticalSection(&particle_job_system.cs_guard);
}

void ParticleJobSystem::SyncUpdateJobs()
{
  EnterCriticalSection(&cs_guard);

  while (complete_entries < total_entries)
  {
    SleepConditionVariableCS(&cv_jobs_complete, &cs_guard, INFINITE);
  }

  LeaveCriticalSection(&cs_guard);
}

void ParticleJobSystem::KickUpdateJobs(ParticleRingBuffer* particle_array, float dt)
{
  EnterCriticalSection(&cs_guard);

  first_unclaimed_entry = 0;
  complete_entries = 0;
  total_entries = particle_array->GetActiveCount();
  stored_particle_array = particle_array;
  stored_dt = dt;

  WakeAllConditionVariable(&cv_wait_for_jobs);

  LeaveCriticalSection(&cs_guard);
}