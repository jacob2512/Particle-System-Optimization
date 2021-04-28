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

void Particle::CopyDataOnly(Particle* p)
{
  // copy the data only
  // this way of copying data is more efficient that element by element
  this->position = p->position;
  this->velocity = p->velocity;
  this->scale = p->scale;
  this->rotation = p->rotation;
  this->rotation_velocity = p->rotation_velocity;
  this->life = p->life;
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


