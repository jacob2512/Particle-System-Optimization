#include "ParticleJobSystem.h"
#include <cassert>
#include <processthreadsapi.h>

void ParticleJobSystem::threadentry()
{

}

ParticleJobSystem::ParticleJobSystem():
  first_unclaimed_entry(0),
  complete_entries(0),
  total_entries(0)
{
  InitializeCriticalSection(&thread_entry_guard);
  InitializeConditionVariable(&thread_entry_wait_state);
  InitializeConditionVariable(&thread_entry_stop_state);

  for (int i = 0; i < max_threads; i++)
  {
    particle_thread_array[i] = std::thread(threadentry);
    
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
  for (int i = 0; i < max_threads; i++)
  {
    particle_thread_array[i].join();
  }
}

void ParticleJobSystem::KickUpdateJobs()
{


}

void ParticleJobSystem::Split()
{

}
