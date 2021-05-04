#include "DO_NOT_MODIFY\Timer\Timer.h"
#include "DO_NOT_MODIFY\Timer\GlobalTimer.h"
#include "DO_NOT_MODIFY\OpenGL\OpenGLInterface.h"

#include <assert.h>

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


ParticleEmitter::ParticleEmitter()
  : last_spawn(globalTimer::getTimerInSec()),
  last_loop(globalTimer::getTimerInSec()),
  last_active_particle(-1),
  vel_variance(1.0f, 4.0f, 0.4f),
  pos_variance(1.0f, 1.0f, 1.0f),
  headParticle(0)
{
  // nothing to do
}



ParticleEmitter::~ParticleEmitter()
{
  // do nothing
}


void ParticleEmitter::SpawnParticle()
{
  // create another particle if there are ones free
  if (last_active_particle < max_particles - 1)
  {

    // create new particle
    Particle* newParticle = new Particle();

    // initialize the particle
    newParticle->life = life;
    newParticle->position = start_position;
    newParticle->velocity = start_velocity;
    newParticle->scale = scale;

    // apply the variance
    this->Execute(newParticle->position, newParticle->velocity, newParticle->scale);

    // increment count
    last_active_particle++;

    // add to list
    this->addParticleToList(newParticle);

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

  Particle* p = this->headParticle;
  // walk the particles

  while (p != 0)
  {
    // call every particle and update its position 
    p->Update(time_elapsed);

    // if it's live is greater that the max_life 
    // and there is some on the list
    // remove node
    if ((p->life > max_life) && (last_active_particle > 0))
    {
      // particle to remove
      Particle* s = p;

      // need to squirrel it away.
      p = p->next;

      // remove last node
      this->removeParticleFromList(s);

      // update the number of particles
      last_active_particle--;
    }
    else
    {
      // increment to next point
      p = p->next;
    }
  }

  //move a copy to vector for faster iterations in draw
  p = this->headParticle;
  bufferCount = 0;

  // clear the buffer
  drawBuffer.clear();

  // walk the pointers, add to list
  while (p != 0)
  {
    // add to buffer
    drawBuffer.push_back(*p);

    // advance ptr
    p = p->next;

    // track the current count
    bufferCount++;
  }

  // make sure the counts track (asserts go away in release - relax Christos)
  assert(bufferCount == (last_active_particle + 1));
  last_loop = current_time;
}

void ParticleEmitter::addParticleToList(Particle* p)
{
  assert(p);
  if (this->headParticle == 0)
  { // first on list
    this->headParticle = p;
    p->next = 0;
    p->prev = 0;
  }
  else
  { // add to front of list
    headParticle->prev = p;
    p->next = headParticle;
    p->prev = 0;
    headParticle = p;
  }

}

void ParticleEmitter::removeParticleFromList(Particle* p)
{
  // make sure we are not screwed with a null pointer
  assert(p);

  if (p->prev == 0)
  {
    if (p->next == 0)
    {
      // only one on the list
      this->headParticle = 0;
    }
    else
    {
      // first on the list
      p->next->prev = 0;
      this->headParticle = p->next;
    }
  }
  else //if (p->prev != 0)
  {
    if (p->next == 0)
    {
      // last on the list 
      p->prev->next = 0;
    }
    else
    {
      // middle of the list
      p->prev->next = p->next;
      p->next->prev = p->prev;
    }
  }

  // bye bye
  delete p;
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

  // iterate throughout the list of particles
  std::list<Particle>::iterator it;
  for (it = drawBuffer.begin(); it != drawBuffer.end(); ++it)
  {
    // particle position
    transParticleMatrix.setTransMatrix(it->position);

    // rotation matrix
    rotParticleMatrix.setRotZMatrix(it->rotation);

    // scale Matrix
    scaleParticleMatrix.setScaleMatrix(it->scale);

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

  // done with buffer, clear it.
  drawBuffer.clear();
}


void ParticleEmitter::Execute(Vect4D& pos, Vect4D& vel, Vect4D& sc)
{
  // Add some randomness...

  // Yes it's ugly - I didn't write this so don't bitch at me
  // Sometimes code like this is inside real commercial code ( so now you know how it feels )

  float* t_pos = reinterpret_cast<float*>(&pos);
  float* t_var = &pos_variance.getxaddress();

  float var = static_cast<float>(rand() % 1000) * 0.001f;
  float sign = static_cast<float>(rand() % 2);

  for (int i = 0; i < 7; i++)
  {
    var = static_cast<float>(rand() % 1000) * 0.001f;
    sign = static_cast<float>(rand() % 2);

    if (i == 3)
    {
      t_pos = &vel.getxaddress();
      t_var = &vel_variance.getxaddress();
    }
    else
    {
      t_pos++;
      t_var++;
    }

    if (i == 6)
    {
      var *= 2.0f;
    }

    if (sign == 0)
    {
      var *= -1.0f;
    }

    if (i == 6)
    {
      var *= 2.0f;
      sc = sc * var;
    }
    else
    {
      *t_pos += *t_var * var;
    }
  }
}


// End of file