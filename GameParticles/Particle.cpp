#include "Particle.h"
#include "Math/Matrix.h"

Particle::Particle()
{
  // constructor
  this->life = 0.0f;
  this->position.set(0.0f, 0.0f, 0.0f);
  this->velocity.set(0.0f, 0.0f, 0.0f);
  this->scale.set(1.0f, 1.0f, 1.0f);
  this->rotation = 0.0f;
  this->rotation_velocity = 1.005f;
}

Particle::~Particle()
{
  // nothing to do
}

void Particle::Update(const float& time_elapsed)
{
  // serious math below - magic secret sauce
  life += time_elapsed;
  position = position + (velocity * time_elapsed);
  v = position.Cross(z_axis);
  position = position + v.norm() * v_offset * life;

  rotation = rotation + rotation_velocity * time_elapsed;
}


// End of file


