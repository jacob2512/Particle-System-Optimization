#include <assert.h>

// Offset to local directory
// REALLY useful trick to insure that you link to your code not some other library
#include "DO_NOT_MODIFY\Timer\Timer.h"
#include "DO_NOT_MODIFY\Timer\GlobalTimer.h"
#include "DO_NOT_MODIFY\Event\EventHandler.h"
#include "DO_NOT_MODIFY\OpenGL\OpenGLInterface.h"
#include "ParticleEmitter.h"


// WIN32 - prototype
int main(int argc, char* const argv[]);

// WinMain required by windows for all win32 applications.
// This is our Windows entry point.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  nCmdShow;
  lpCmdLine;
  hPrevInstance;
  OpenGLDevice::SetHInstance(hInstance);
  main(__argc, __argv);
}

// main program
int main(int argc, char* const argv[])
{
  argc;
  argv;
  srand(1);

  // initialize timers:------------------------------
    // Initialize timer
  timer::initTimer();

  // Create a global Timer
  globalTimer::create();

  // Create a timer objects
  timer updateTimer;
  timer drawTimer;

  // create a window:---------------------------------
  const bool success = OpenGLDevice::InitWindow();
  assert(success);

  // create an emitter:-------------------------------
  ParticleEmitter emitter;

  // Get the inverse Camera Matrix:-------------------

    // initialize the camera matrix
  Matrix cameraMatrix;
  cameraMatrix.setIdentMatrix();

  // setup the translation matrix
  Matrix transMatrix;
  Vect4D trans(0.0f, 3.0f, 10.0f);
  transMatrix.setTransMatrix(&trans);

  // multiply them together
  Matrix offsetCameraMatrix;
  offsetCameraMatrix = cameraMatrix * transMatrix;

  // counter for printing
  int i = 0;
  int j = 0;
  float totUpdateTime = 0.0f;

  // main update loop... do this forever or until some breaks
  while (OpenGLDevice::IsRunning())
  {
    // start update timer ---------------------------------------
    updateTimer.tic();

    // start draw... end draw (the draw updates)
    OpenGLDevice::StartDraw();

    // set matrix to Model View
    glMatrixMode(GL_MODELVIEW);
    // push the inverseCameraMarix to stack
    glLoadMatrixf(reinterpret_cast<float*>(&offsetCameraMatrix.Inverse()));
    // push the camera matrix
    glPushMatrix();

    // update the emitter
    emitter.update();

    // stop update timer: -----------------------------------------
    updateTimer.toc();

    // start draw timer: ----------------------------------------
    drawTimer.tic();

    // draw particles
    emitter.draw();

    // pop matrix - needs to correspond to previous push
    glPopMatrix();

    // stop draw timer: -----------------------------------------
    drawTimer.toc();

    // finish draw update
    OpenGLDevice::EndDraw();

    // Love for Windows - allows the windows to behave to external events
    EventHandler::ProcessEvents();

    i++;
    // update output every 50 times
    if (i > 50)
    {
      i = 0;
      j++;
      const float updateTime = updateTimer.timeInSeconds() * 1000.0f;
      const float drawTime = drawTimer.timeInSeconds() * 1000.0f;
      const float totTime = updateTime + drawTime;
      totUpdateTime += updateTime;

      if (j > 60)
      {
        float avgUpdateTime = totUpdateTime / j;
        j = 0;
        totUpdateTime = 0.0f;
        printf("LoopTime: update:%f ms  draw:%f ms  tot:%f ms avg:%f ms\n", updateTime, drawTime, totTime, avgUpdateTime);
      }
      else
      {
        printf("LoopTime: update:%f ms  draw:%f ms  tot:%f ms\n", updateTime, drawTime, totTime);
      }
    }
  }

  return 0;
}


// End of file