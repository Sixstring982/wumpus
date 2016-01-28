/** wumpus
 *  Copyright 2016 sixstring982, all rights reserved.
 *
 *  File: GLMapRenderer.cpp
 *  Purpose: A map renderer which uses OpenGL shaders.
 */
#include "./inc/GLMapRenderer.hpp"

#include <SDL2/SDL.h>
#include "./inc/OpenGL.hpp"

#include "./inc/ShaderProgram.hpp"

namespace wumpus {
void* startThreadHelper(void* glRenderer) {
  GLMapRenderer* renderer = reinterpret_cast<GLMapRenderer*>(glRenderer);
  renderer->threadLoop();
  pthread_exit(NULL);
}

GLMapRenderer::GLMapRenderer() :
  roomType(ROOM_EMPTY),
  thread(0),
  running(false),
  enteredCave(false) { }

GLMapRenderer::~GLMapRenderer() {
  if (isRunning()) {
    stop();
  }
}

int  GLMapRenderer::start() {
  if (isRunning()) {
    return -1;
  } else {
    /* Setup mutexes */
    if (pthread_mutex_init(&roomTypeLock, NULL) != 0) {
      return -1;
    } else if (pthread_mutex_init(&runningLock, NULL) != 0) {
      pthread_mutex_destroy(&roomTypeLock);
      return -1;
    } else if (pthread_mutex_init(&enteredCaveLock, NULL) != 0) {
      pthread_mutex_destroy(&roomTypeLock);
      pthread_mutex_destroy(&runningLock);
      return -1;
    } else if (pthread_create(&thread, NULL, startThreadHelper,
                              reinterpret_cast<void*>(this)) != 0) {
      pthread_mutex_destroy(&roomTypeLock);
      pthread_mutex_destroy(&runningLock);
      pthread_mutex_destroy(&enteredCaveLock);
      return -1;
    }

    setRunning(true);
    return 0;
  }
}

int GLMapRenderer::stop() {
  if (!isRunning()) {
    return -1;
  } else {
    void* value;

    setRunning(false);
    pthread_join(thread, &value);

    pthread_mutex_destroy(&roomTypeLock);
    pthread_mutex_destroy(&runningLock);
    pthread_mutex_destroy(&enteredCaveLock);

    return 0;
  }
}

bool GLMapRenderer::isRunning() {
  bool retVal;
  pthread_mutex_lock(&runningLock); {
    /* CRITICAL SECTION */
    retVal = running;
  } pthread_mutex_unlock(&runningLock);

  return retVal;
}

bool GLMapRenderer::hasEnteredCave() {
  bool retVal;
  pthread_mutex_lock(&enteredCaveLock); {
    /* CRITICAL SECTION */
    retVal = enteredCave;
  } pthread_mutex_unlock(&enteredCaveLock);

  return retVal;
}

RoomType GLMapRenderer::getRoomType() {
  RoomType type;
  pthread_mutex_lock(&roomTypeLock); {
    /* CRITICAL SECTION */
    type = roomType;
  } pthread_mutex_unlock(&roomTypeLock);

  return type;
}

void GLMapRenderer::renderMap(const Map& map, uint32_t) {
  setEnteredCave(true);
  setRoomType(map.getCurrentRoomType());
}

void GLMapRenderer::renderLootResult(LootResult) const { }

void GLMapRenderer::renderMoveResult(MoveResult) const { }

void GLMapRenderer::threadLoop() {
  const uint32_t SCREEN_WIDTH = 640;
  const uint32_t SCREEN_HEIGHT = 480;
  RoomType type = getRoomType();
  SDL_Window* window;
  SDL_GLContext glContext;
  ShaderProgram innProgram;
  ShaderProgram entranceProgram;
  ShaderProgram* program;
  SDL_Event event;

  SDL_Init(SDL_INIT_VIDEO);

  window = SDL_CreateWindow("Hunt The Wumpus",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            SCREEN_WIDTH, SCREEN_HEIGHT,
                            SDL_WINDOW_OPENGL);

  if (window == NULL) {
    fprintf(stderr, "GLMapRenderer::threadLoop -- "
                    "Could not create GL Window.\n");
    SDL_Quit();
    return;
  }

  glContext = SDL_GL_CreateContext(window);

  /* Get GLEW up and running */
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    fprintf(stderr, "Could not initialize glew.\n");
    SDL_DestroyWindow(window);
    SDL_GL_DeleteContext(glContext);

    SDL_Quit();
    return;
  }

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if (innProgram.load(SHADER_ROOT "/default.vert",
                      SHADER_ROOT "/inn.frag") == -1) {
    SDL_DestroyWindow(window);
    SDL_GL_DeleteContext(glContext);

    SDL_Quit();
     return;
  }

  if (entranceProgram.load(SHADER_ROOT "/default.vert",
                           SHADER_ROOT "/entrance.frag") == -1) {
    innProgram.unload();
    SDL_DestroyWindow(window);
    SDL_GL_DeleteContext(glContext);

    SDL_Quit();
     return;
  }

  program = &innProgram;

  glColor3f(1.0f, 1.0f, 1.0f);
  while (isRunning()) {
    /* Eat all SDL Events to keep the OS from complaining */
    while (SDL_PollEvent(&event)) {
      continue;
    }
    if (hasEnteredCave()) {
      RoomType type2 = getRoomType();
      if (type2 != type) {
        type = type2;
        // Load the other shader
        switch (type) {
        case ROOM_ENTRANCE: program = &entranceProgram; break;
        case ROOM_WUMPUS:
        case ROOM_PIT:
        case ROOM_GOLD:
        case ROOM_WEAPON:
        case ROOM_EMPTY:
        case ROOM_WALL:
          program = &innProgram;
          break;
        }
      }
    }
    glClear(GL_COLOR_BUFFER_BIT);

    /* Draw shader region */
    program->use();
    program->setVector2Uniform("iResolution",
                               Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));
    program->setFloatUniform("iGlobalTime",
                             SDL_GetTicks() / 1000.0f);
    glBegin(GL_TRIANGLES); {
      glVertex3f(1.0f, 1.0f, 0.0f);
      glVertex3f(-1.0f, 1.0f, 0.0f);
      glVertex3f(1.0f, -1.0f, 0.0f);

      glVertex3f(-1.0f, -1.0f, 0.0f);
      glVertex3f(1.0f, -1.0f, 0.0f);
      glVertex3f(-1.0f, 1.0f, 0.0f);
    } glEnd();

    SDL_GL_SwapWindow(window);
    SDL_Delay(17 /* 60 fps */);
  }

  entranceProgram.unload();
  innProgram.unload();

  SDL_DestroyWindow(window);
  SDL_GL_DeleteContext(glContext);

  SDL_Quit();
}

void GLMapRenderer::setRunning(bool value) {
  pthread_mutex_lock(&runningLock); {
    /* CRITICAL SECTION */
    this->running = value;
  } pthread_mutex_unlock(&runningLock);
}

void GLMapRenderer::setEnteredCave(bool value) {
  pthread_mutex_lock(&enteredCaveLock); {
    /* CRITICAL SECTION */
    this->enteredCave = value;
  } pthread_mutex_unlock(&enteredCaveLock);
}

void GLMapRenderer::setRoomType(RoomType value) {
  pthread_mutex_lock(&roomTypeLock); {
    /* CRITICAL SECTION */
    this->roomType = value;
  } pthread_mutex_unlock(&roomTypeLock);
}
}  // namespace wumpus
