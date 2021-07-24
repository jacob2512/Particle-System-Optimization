#pragma once

// include
#include "Math/Vect4D.h"

//class size: 68 bytes
class Particle
{
public:
  friend class ParticleEmitter;

  Particle();
  ~Particle();
  void Update(const float& time_elapsed);

private:

  Vect4D	position;
  Vect4D	velocity;
  Vect4D	scale;

  float	life;
  float	rotation;
  float	rotation_velocity;
  static constexpr float v_offset = 0.05f;

  Particle* next;
  Particle* prev;
};
