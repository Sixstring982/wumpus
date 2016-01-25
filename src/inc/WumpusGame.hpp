/** wumpus
 *  Copyright 2016 sixstring982, all rights reserved.
 *
 *  File: WumpusGame.hpp
 *  Purpose: Class which contains all state in a Wumpus game.
 */
#ifndef WUMPUS_SRC_INC_WUMPUSGAME_HPP_
#define WUMPUS_SRC_INC_WUMPUSGAME_HPP_

#include "./MapRenderer.hpp"
#include "./Input.hpp"

namespace wumpus {
class WumpusGame {
 private:
  MapRenderer* renderer;
  uint32_t points;

 public:
  WumpusGame();

  /**
   *  Sets the MapRenderer used to render the Wumpus
   *  Map.
   */
  void setMapRenderer(MapRenderer* mapRenderer);

  /**
   *  Starts a new game of "Hunt the Wumpus".
   */
  void start();

 private:
  int gameLoop(const Input& input, Map* map);
};
}  // namespace wumpus

#endif  // WUMPUS_SRC_INC_WUMPUSGAME_HPP_
