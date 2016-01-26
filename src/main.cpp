/** wumpus
 *  Copyright 2016 sixstring982, all rights reserved.
 *
 *  File: main.cpp
 *  Purpose: Main entry point to Wumpus game.
 */
#include <stdio.h>

#include "./inc/WumpusGame.hpp"
#include "./inc/CLMapRenderer.hpp"
#include "./inc/TeeMapRenderer.hpp"
#include "./inc/GLMapRenderer.hpp"

namespace wumpus {
  /* Prototype; gets rid of clang warning */
  int main();

  int main() {
    WumpusGame game;
    CLMapRenderer clRenderer;
    GLMapRenderer glRenderer;
    TeeMapRenderer renderer(&clRenderer, &glRenderer);

    glRenderer.start();

    game.setMapRenderer(&renderer);
    game.start();

    glRenderer.stop();
    return 0;
  }
}  // namespace wumpus

int main() {
  return wumpus::main();
}
