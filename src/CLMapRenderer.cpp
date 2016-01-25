/** wumpus
 *  Copyright 2016 sixstring982, all rights reserved.
 *
 *  File: CLMapRenderer.cpp
 *  Purpose: A class which renders a Wumpus Map to stdout.
 */
#include "./inc/CLMapRenderer.hpp"

#include <stdio.h>

namespace wumpus {
CLMapRenderer::~CLMapRenderer() { }

char CLMapRenderer::charFromRoomType(RoomType type, bool explored) const {
  if (!explored) {
    return '?';
  } else {
    switch (type) {
    case ROOM_ENTRANCE: return '^';
    case ROOM_WUMPUS: return '~';
    case ROOM_PIT: return 'P';
    case ROOM_GOLD: return '$';
    case ROOM_WEAPON: return 'W';
    case ROOM_EMPTY: return '.';
    case ROOM_WALL: return '#';
    }
  }
}

void CLMapRenderer::printDescription(RoomType type) const {
  switch (type) {
  case ROOM_ENTRANCE:
    printf("You are at the cave entrance. You wish to\n");
    printf("run away?\n");
    break;
  case ROOM_WUMPUS:
    printf("Overwhelmed in stench, a Wumpus stands before\n");
    printf("you. It looks hungry.\n");
    break;
  case ROOM_PIT:
    printf("You fall into a pit! You fall, and fall, and fall...\n");
    break;
  case ROOM_GOLD:
    printf("A small pile of gold is on the floor. It looks like it\n");
    printf("came from an adventurer who fed a Wumpus recently.\n");
    break;
  case ROOM_WEAPON:
    printf("A large club leans against the wall.\n");
    break;
  case ROOM_EMPTY:
    printf("You stand in the cave.\n");
    break;
  case ROOM_WALL:
    printf("How did you get inside the wall?\n");
    break;
  }
}

void CLMapRenderer::printClues(const Map& map) const {
  RoomType adj[4];

  map.fillAdjacentRooms(adj);

  bool wumpus = false;
  bool pit = false;

  for (uint32_t i = 0; i < 4; i++) {
    if (adj[i] == ROOM_PIT) {
      pit = true;
    } else if (adj[i] == ROOM_WUMPUS) {
      wumpus = true;
    }
  }

  if (wumpus) {
    printf("You detect a foul stench in the air...\n");
  }
  if (pit) {
    printf("You hear a howling wind...\n ");
  }
}

void CLMapRenderer::renderMap(const Map& map) const {
  Vector2 player;
  Vector2 roomV;
  map.fillPlayerPosition(&player);

  printf("\n");
  for (int32_t y = 1; y >= -1; y--) {
    for (int32_t x = -1; x <= 1; x++) {
      if (x == 0 && y == 0) {
        printf("@");
      } else {
        roomV.set(player);
        roomV.add(Vector2(x, y));

        RoomType r = map.getRoomType(roomV);
        bool explored = map.getHasBeenExplored(roomV);

        printf("%c", charFromRoomType(r, explored));
      }
    }
    printf("\n");
  }

  printDescription(map.getRoomType(player));
}
}  // namespace wumpus
