#include "DO_NOT_MODIFY\Timer\Timer.h"
#include "DO_NOT_MODIFY\Timer\GlobalTimer.h"
#include "DO_NOT_MODIFY\OpenGL\OpenGLInterface.h"

#include "CustomAssert.h"

#include "ParticleEmitter.h"

static constexpr unsigned char squareColors[] =
{
  255,  255,  000,  255,
  000,  255,  255,  255,
  000,  000,  000,  000,
  255,  000,  255,  255,
};

static constexpr float squareVertices[] =
{
  -0.015f,  -0.015f, 0.0f,
   0.015f,  -0.015f, 0.0f,
  -0.015f,   0.015f, 0.0f,
   0.015f,   0.015f, 0.0f,
};

constexpr Vect4D	start_position(0.0f, 0.0f, 0.0f, 1.0f);
constexpr Vect4D  start_velocity( 0.0f, 1.0f, 0.0f, 1.0f );
constexpr Vect4D  start_scale( 2.0f, 2.0f, 2.0f, 1.0f );

constexpr Vect4D	vel_variance( 1.0f, 4.0f, 0.4f, 1.0f );
constexpr Vect4D	pos_variance( 1.0f, 1.0f, 1.0f, 1.0f );

ParticleEmitter::ParticleEmitter()
  : last_spawn(globalTimer::getTimerInSec()),
  last_loop(globalTimer::getTimerInSec()),
  particle_array()
{
  particle_array_size = -1;
}



ParticleEmitter::~ParticleEmitter()
{
  // do nothing
}


void ParticleEmitter::SpawnParticle()
{
  // create another particle if there are ones free
  if (particle_array_size < particle_heap.GetMaxPatricles())
  {
    // create new particle
    Particle* newParticle = new (particle_heap.ParticleAlloc(particle_heap.GetParticleSize(), 16)) Particle;
    
    if (newParticle)
    {
      // initialize the particle
      newParticle->life = life;
      newParticle->position = start_position;
      newParticle->velocity = start_velocity;
      newParticle->scale = start_scale;

      // apply the variance
      this->Execute(newParticle->position, newParticle->velocity, newParticle->scale);

      // add to array
      this->addParticleToArray(newParticle);
    }

  }
}

void ParticleEmitter::update()
{
  // get current time
  float current_time = globalTimer::getTimerInSec();

  // spawn particles
  float time_elapsed = current_time - last_spawn;

  // update
  while (spawn_frequency < time_elapsed)
  {
    // spawn a particle
    this->SpawnParticle();
    // adjust time
    time_elapsed -= spawn_frequency;
    // last time
    last_spawn = current_time;
  }

  // total elapsed
  time_elapsed = current_time - last_loop;

  // walk the particles
  for (int i = particle_array_size; i >= 0; i--)
  {
    Particle* p = particle_array[i];

    // call every particle and update its position 
    p->Update(time_elapsed);

    // if it's live is greater that the max_life 
    // and there are some in the array
    // remove element
    if ((p->life > max_life) && (i > 0))
    {
      this->removeParticleFromArray(i);
    }
  }

  last_loop = current_time;
}

void ParticleEmitter::addParticleToArray(Particle* p)
{
  // increment count
  particle_array_size++;

  ASSERT(p);
  particle_array[particle_array_size] = p;
}

void ParticleEmitter::removeParticleFromArray(int index)
{
  Particle* p = particle_array[index];

  // make sure we are not screwed with a null pointer
  ASSERT(p);

  p->~Particle();

  // bye bye
  particle_heap.ParticleFree(p);

  // update the number of particles
  particle_array_size--;
}

void ParticleEmitter::draw()
{
  // OpenGL goo... don't worry about this
  glVertexPointer(3, GL_FLOAT, 0, squareVertices);
  glEnableClientState(GL_VERTEX_ARRAY);
  glColorPointer(4, GL_UNSIGNED_BYTE, 0, squareColors);
  glEnableClientState(GL_COLOR_ARRAY);

  // get the camera matrix from OpenGL
  // need to get the position
  Matrix cameraMatrix;

  Matrix transCameraMatrix;

  Matrix transParticleMatrix;
  Matrix rotParticleMatrix;
  Matrix scaleParticleMatrix;

  float* cameraFloatArray = cameraMatrix.asFloatArray();

  // get the camera matrix from OpenGL
  glGetFloatv(GL_MODELVIEW_MATRIX, cameraFloatArray);
  cameraMatrix.setFromFloatArray(cameraFloatArray);
  transCameraMatrix.setTransMatrix(cameraMatrix.getTransRow());

  // walk the particles
  for (int i = particle_array_size; i >= 0; i--)
  {
    Particle* p = particle_array[i];

    // particle position
    transParticleMatrix.setTransMatrix(p->position);

    // rotation matrix
    rotParticleMatrix.setRotZMatrix(p->rotation);

    // scale Matrix
    scaleParticleMatrix.setScaleMatrix(p->scale);

    // total transformation of particle
    Matrix resultMatrix;
    resultMatrix = scaleParticleMatrix * transCameraMatrix * transParticleMatrix * rotParticleMatrix * scaleParticleMatrix;

    // set the transformation matrix
    glLoadMatrixf(resultMatrix.asFloatArray());

    // draw the triangle strip
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // clears or flushes some internal setting, used in debug, but is need for performance reasons
    // magic...  really it's magic.
    glGetError();
  }
}

inline static float GenerateRandom()
{
  return 2.0f * rand() / (float)RAND_MAX - 1;
}

void ParticleEmitter::Execute(Vect4D& pos, Vect4D& vel, Vect4D& sc)
{
  // Add some randomness...

  Vect4D pos_rand_var(GenerateRandom(), GenerateRandom(), GenerateRandom());
  pos_rand_var *= pos_variance;
  Vect4D vel_rand_var(GenerateRandom(), GenerateRandom(), GenerateRandom());
  vel_rand_var *= vel_variance;

  pos += pos_rand_var;
  vel += vel_rand_var;

  sc *= GenerateRandom() * 2;
}

// End of file