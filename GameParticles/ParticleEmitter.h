#pragma once

#include "Math\Matrix.h"
#include "ParticleRingBuffer.h"
#include "ParticleHeap.h"
#include "Particle.h"

#include <list>

//class size: 60 bytes
class ParticleEmitter
{
public:
  ParticleEmitter();
  ~ParticleEmitter();

  void AddParticleToArray();
  void Execute(Vect4D& pos, Vect4D& vel, Vect4D& sc);
  void update();
  void draw();

private:

  static constexpr float  life = 0.0f;
  static constexpr float	max_life = 10.0f;
  static constexpr float	spawn_frequency = 0.0000001f;

  float	last_spawn;
  float	last_loop;

  // added for speed efficiency
  static constexpr float	scale_variance = 2.5f;

  ParticleRingBuffer particle_array;
};
