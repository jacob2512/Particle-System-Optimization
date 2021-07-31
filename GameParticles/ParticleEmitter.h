#pragma once

#include "Math\Matrix.h"
#include "ParticleHeap.h"
#include "Particle.h"

#include <list>

//class size: 60 bytes
class ParticleEmitter
{
public:
  ParticleEmitter();
  ~ParticleEmitter();

  void SpawnParticle();
  void update();
  void draw();

  void addParticleToArray(Particle* p);
  void removeParticleFromArray(int index);

  void Execute(Vect4D& pos, Vect4D& vel, Vect4D& sc);

private:

  static constexpr float  life = 0.0f;
  static constexpr float	max_life = 10.0f;
  static constexpr float	spawn_frequency = 0.0000001f;

  float	last_spawn;
  float	last_loop;
  int		particle_array_size; //change to 0 -> should be in ring buffer

  // added for speed efficiency
  static constexpr float	scale_variance = 2.5f;

  Particle* particle_array[ParticleHeap::GetMaxPatricles()];
  ParticleHeap particle_heap;
};
