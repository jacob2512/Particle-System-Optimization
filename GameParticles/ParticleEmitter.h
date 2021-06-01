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

  void addParticleToList(Particle* p);
  void removeParticleFromList(Particle* p);

  void Execute(Vect4D& pos, Vect4D& vel, Vect4D& sc);

private:

  static constexpr float life = 0.0f;
  static constexpr float	max_life = 10.0f;
  static constexpr float	spawn_frequency = 0.0000001f;

  float	last_spawn;
  float	last_loop;
  int		last_active_particle = -1;
  static constexpr int		max_particles = 30 * 1000;

  // added for speed efficiency
  static constexpr float	scale_variance = 2.5f;
  std::list<Particle> drawBuffer;
  Particle* headParticle;
  ParticleHeap particle_heap;

};
