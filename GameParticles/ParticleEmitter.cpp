#include "DO_NOT_MODIFY\Timer\Timer.h"
#include "DO_NOT_MODIFY\Timer\GlobalTimer.h"
#include "DO_NOT_MODIFY\OpenGL\OpenGLInterface.h"

#include "CustomAssert.h"

#include "ParticleEmitter.h"
#include "ParticleJobSystem.h"

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
  particle_array(particle_heap.GetMaxParticles())
{
}



ParticleEmitter::~ParticleEmitter()
{
  // do nothing
}


void ParticleEmitter::AddParticleToArray()
{
    Particle* newParticle = particle_array.Allocate();

    if (newParticle)
    {
      // initialize the particle
      newParticle->life = life;
      newParticle->position = start_position;
      newParticle->velocity = start_velocity;
      newParticle->scale = start_scale;

      // apply the variance
      this->Execute(newParticle->position, newParticle->velocity, newParticle->scale);
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
    AddParticleToArray();
    // adjust time
    time_elapsed -= spawn_frequency;
    // last time
    last_spawn = current_time;
  }

  // total elapsed
  time_elapsed = current_time - last_loop;

  particle_job_system.KickUpdateJobs(&particle_array, time_elapsed);

  // walk the particles
  for (int i = 0; i < particle_array.GetActiveCount(); i++)
  {
    Particle* p = particle_array.GetAt(i);

    // if it's live is greater that the max_life 
    // and there are some in the array
    // remove element
    if ((p->life > max_life))
    {
      particle_array.Remove();
      i--;
    }
  }

  last_loop = current_time;
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
  for (int i = particle_array.GetActiveCount()-1; i >= 0; i--)
  {
    Particle* p = particle_array.GetAt(i);

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