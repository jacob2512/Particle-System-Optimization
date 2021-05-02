#pragma once

// include
#include "Math/Vect4D.h"

class Particle
{
public:
  friend class ParticleEmitter;

  Particle();
  ~Particle();
  void Update(const float& time_elapsed);
private:
  Particle* next;
  Particle* prev;

  float	life;
  Vect4D	position;
  Vect4D	velocity;
  Vect4D	scale;
  float	rotation;
  float	rotation_velocity;

  Vect4D z_axis = Vect4D(0.0f, -0.25f, 1.0f);
  Vect4D v = Vect4D(3, 4, 0);
  static constexpr float v_offset = 0.05f;
};
