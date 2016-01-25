/** wumpus
 *  Copyright 2016 sixstring982, all rights reserved.
 *
 *  File: WumpusGame.cpp
 *  Purpose: Class which contains all state in a Wumpus game.
 */
#include "./inc/WumpusGame.hpp"

#include <stdio.h>

namespace wumpus {
WumpusGame::WumpusGame() :
  renderer(NULL), points(0) { }

void WumpusGame::setMapRenderer(MapRenderer* mapRenderer) {
  this->renderer = mapRenderer;
}

void WumpusGame::start() {
  Input input;
  uint32_t size = 0;
  Map map;
  bool setup = false;

  printf("So, you want to hunt the Wumpus... DO YA?\n");
  printf("Well, if you know enough about the Wumpus to hunt him,\n");
  printf("you must know how big his cave is...\n\n");

  while (!setup) {
    printf("(How big of a cave do you want to play with?) > ");
    if (input.readUint32(&size) == -1) {
      printf("Quit pulling my leg! How big is his cave??\n\n");
    } else if (map.setup(size) == -1) {
      printf("No cave is that big! How big is his cave??\n\n");
    } else {
      setup = true;
    }
  }

  if (size < 5) {
    printf("Oh it's that small, huh? Don't get cornered!\n");
  } else if (size < 10) {
    printf("Or so they say... Seems there are lots of hiding\n");
    printf("places in there.\n");
  } else {
    printf("It's that large?? God have mercy on you.\n");
  }

  printf("I hope you brought a weapon with you, as well. Many\n");
  printf("have tried to bring the Wumpus down with their bare\n");
  printf("hands and have died trying. Good luck!\n\n");

  printf("Press Enter to embark towards the cave...\n");
  input.pause();

  /* Do the game loop... */
  gameLoop(input, &map);

  printf("You earned %u points.\n", points);
}

static void listMoves() {
  printf("You may make the following moves:\n\n");
  printf("[?] Ask for help.\n");
  printf("[N] Enter the chamber to the North.\n");
  printf("[E] Enter the chamber to the East.\n");
  printf("[S] Enter the chamber to the South.\n");
  printf("[W] Enter the chamber to the West.\n");
  printf("[L] Loot the current chamber.\n");
  printf("[R] Run from the cave you are near the exit.\n");
  printf("[X] Exit the game, forfeiting all of your points.\n");
}

int WumpusGame::gameLoop(const Input& input, Map* map) {
  bool running = true;
  Command cmd;
  while (running) {
    renderer->renderMap(*map);

    if (input.readCommand(&cmd) == -1) {
      printf("I don't understand that command.\n");
    } else {
      switch (cmd) {
      case COMMAND_LIST: listMoves(); break;
      case COMMAND_HALT:
        points = 0;
        running = false;
        break;
      case COMMAND_NORTH:
      case COMMAND_EAST:
      case COMMAND_SOUTH:
      case COMMAND_WEST:
      case COMMAND_LOOT:
      case COMMAND_RUN:
        printf("I understand that!\n");
        break;
      }
    }
  }
  return 0;
}
}  // namespace wumpus
