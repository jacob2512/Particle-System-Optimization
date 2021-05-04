#pragma once

// include
#include "Math/Vect4D.h"

//class size: 84 bytes (68 bytes, without Vect4D z_axis)
class Particle
{
public:
  friend class ParticleEmitter;

  Particle();
  ~Particle();
  void Update(const float& time_elapsed);

private:

  float	life;
  float	rotation;
  float	rotation_velocity;
  static constexpr float v_offset = 0.05f;
  //Vect4D z_axis = Vect4D(0.0f, -0.25f, 1.0f);

  Vect4D	position;
  Vect4D	velocity;
  Vect4D	scale;

  Particle* next;
  Particle* prev;
};
