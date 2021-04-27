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
  void CopyDataOnly(Particle* p);
private:
  Particle* next;
  Particle* prev;

  Vect4D	prev_Row0;
  Vect4D	prev_Row1;
  Vect4D  prev_Row2;
  Vect4D  prev_Row3;

  Vect4D	position;
  float	life;
  Vect4D	velocity;
  Vect4D	scale;
  float	rotation;
  Vect4D  diff_Row0;
  Vect4D  diff_Row1;
  Vect4D  diff_Row2;
  Vect4D  diff_Row3;
  float	rotation_velocity;

  Vect4D	curr_Row0;
  Vect4D	curr_Row1;
  Vect4D  curr_Row2;
  Vect4D  curr_Row3;

  Vect4D z_axis = Vect4D(0.0f, -0.25f, 1.0f);
  Vect4D v = Vect4D(3, 4, 0);
  static constexpr float v_offset = 0.05f;
};
