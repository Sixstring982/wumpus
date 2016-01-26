/** wumpus
 *  Copyright 2016 sixstring982, all rights reserved.
 *
 *  File: GLMapRenderer.hpp
 *  Purpose: A map renderer which uses OpenGL shaders.
 */
#ifndef WUMPUS_SRC_INC_GLMAPRENDERER_HPP_
#define WUMPUS_SRC_INC_GLMAPRENDERER_HPP_

#include <pthread.h>

#include "./MapRenderer.hpp"

namespace wumpus {
class GLMapRenderer : public MapRenderer {
 private:
  RoomType roomType;
  pthread_t thread;
  pthread_mutex_t enteredCaveLock;
  pthread_mutex_t roomTypeLock;
  pthread_mutex_t runningLock;
  bool running;
  bool enteredCave;

 public:
  GLMapRenderer();

  virtual ~GLMapRenderer();

  int start();

  int stop();

  bool isRunning();

  bool hasEnteredCave();

  RoomType getRoomType();

  virtual void renderMap(const Map& map, uint32_t points);

  virtual void renderLootResult(LootResult result) const;

  virtual void renderMoveResult(MoveResult result) const;

 private:
  void threadLoop();

  void setRunning(bool value);

  void setRoomType(RoomType value);

  void setEnteredCave(bool value);

  friend void* startThreadHelper(void* glRenderer);
};
}  // namespace wumpus

#endif  // WUMPUS_SRC_INC_GLMAPRENDERER_HPP_
