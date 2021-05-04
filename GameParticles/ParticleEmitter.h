#pragma once

#include "Math\Matrix.h"
#include "Math\Vect4D.h"
#include "Particle.h"

#include <list>

//class size: 108 bytes
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
  const Vect4D	start_position = Vect4D(0.0f, 0.0f, 0.0f);
  const Vect4D	start_velocity = Vect4D(0.0f, 1.0f, 0.0f);
  const Vect4D  scale = Vect4D(1.0f, 1.0f, 1.0f);

  static constexpr float life = 0.0f;
  static constexpr float	max_life = 10.0f;
  static constexpr float	spawn_frequency = 0.0000001f;

  float	last_spawn;
  float	last_loop;
  int		last_active_particle;
  static constexpr int		max_particles = 30 * 1000;

  // added for speed efficiency
  static constexpr float	scale_variance = 2.5f;
  Vect4D	vel_variance;
  Vect4D	pos_variance;
  int bufferCount;
  std::list<Particle> drawBuffer;
  Particle* headParticle;

};
