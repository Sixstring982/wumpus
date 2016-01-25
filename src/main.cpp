/** wumpus
 *  Copyright 2016 sixstring982, all rights reserved.
 *
 *  File: main.cpp
 *  Purpose: Main entry point to Wumpus game.
 */
#include <stdio.h>

#include "./inc/WumpusGame.hpp"
#include "./inc/CLMapRenderer.hpp"

namespace wumpus {
  /* Prototype; gets rid of clang warning */
  int main();

  int main() {
    WumpusGame game;
    CLMapRenderer renderer;

    game.setMapRenderer(&renderer);
    game.start();
    return 0;
  }
}  // namespace wumpus

int main() {
  return wumpus::main();
}
