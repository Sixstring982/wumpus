/** wumpus
 *  Copyright 2016 sixstring982, all rights reserved.
 *
 *  File: Map.cpp
 *  Purpose: The Map that will be explored in the Wumpus game.
 */
#include "./inc/Map.hpp"

#include <stdlib.h> /* For free, malloc, rand, srand */
#include <stdio.h>  /* For NULL */
#include <time.h>   /* For time */

namespace wumpus {
const uint32_t Map::MAX_SIZE = 100;
Map::Map() :
  explored(NULL), rooms(NULL),
  playerPos(0, 0), size(0) { }

Map::~Map() {
  if (rooms != NULL) {
    free(rooms);
    free(explored);
  }
}

RoomType Map::getRoomType(const Vector2& pos) const {
  uint32_t idx = 0;
  if (indexFromVector2(pos, &idx) == -1) {
    return ROOM_WALL;
  } else {
    return rooms[idx];
  }
}

bool Map::getHasBeenExplored(const Vector2& pos) const {
  uint32_t idx = 0;
  if (indexFromVector2(pos, &idx) == -1) {
    return true;
  } else {
    return explored[idx];
  }
}

void Map::fillPlayerPosition(Vector2* into) const {
  into->set(playerPos);
}

int Map::movePlayerToEntrance() {
  Vector2 v;
  for (uint32_t x = 0; x < size; x++) {
    for (uint32_t y = 0; y < size; y++) {
      v.set(Vector2(x, y));
      if (getRoomType(v) == ROOM_ENTRANCE) {
        playerPos.set(v);
        return 0;
      }
    }
  }
  return -1;
}

void Map::setRoom(const Vector2& pos, RoomType type) {
  uint32_t idx = 0;
  if (indexFromVector2(pos, &idx) != -1) {
    rooms[idx] = type;
  }
}

void Map::convertEmptyRooms(unsigned int* seed, RoomType type, uint32_t count) {
  Vector2 pos;
  for (uint32_t i = 0; i < count;) {
    pos.set(Vector2(rand_r(seed) % static_cast<int32_t>(size),
                    rand_r(seed) % static_cast<int32_t>(size)));
    if (getRoomType(pos) == ROOM_EMPTY) {
      setRoom(pos, type);
      i++;
    }
  }
}

void Map::layoutRooms() {
  /* Calculate percentage of rooms */
  uint32_t size2 = size * size;
  uint32_t entrances = 1;
  uint32_t wumpus = static_cast<uint32_t>(size2 * 0.15); /* 15% */
  uint32_t pits = static_cast<uint32_t>(size2 * 0.5); /* 5% */
  uint32_t gold = wumpus; /* 15% */
  uint32_t weapon = wumpus; /* 15% */

  /* Gonna use rand_r, so we need a seed. */
  unsigned int seed = static_cast<unsigned int>(time(NULL));

  /* First, set them all to unexplored and empty. */
  for (uint32_t i = 0; i < size2; i++) {
    rooms[i] = ROOM_EMPTY;
    explored[i] = false;
  }

  /* Next, lay out each of the types. */
  convertEmptyRooms(&seed, ROOM_ENTRANCE, entrances);
  convertEmptyRooms(&seed, ROOM_WUMPUS, wumpus);
  convertEmptyRooms(&seed, ROOM_PIT, pits);
  convertEmptyRooms(&seed, ROOM_GOLD, gold);
  convertEmptyRooms(&seed, ROOM_WEAPON, weapon);
}

int Map::setup(uint32_t newSize) {
  if (newSize > MAX_SIZE) {
    return -1;
  }

  uint32_t size2 = newSize * newSize;

  rooms = reinterpret_cast<RoomType*>(
            malloc(sizeof(RoomType) * size2));

  if (rooms == NULL) {
    return -1;
  } else {
    explored = reinterpret_cast<bool*>(
                 malloc(sizeof(bool) * size2));
    if (explored == NULL) {
      free(rooms);
      rooms = NULL;
      return -1;
    } else {
      this->size = newSize;
      layoutRooms();
      return movePlayerToEntrance();
    }
  }
}

void Map::fillAdjacentRooms(RoomType* types) const {
  types[0] = getRoomType(Vector2(0, 1).add(playerPos));
  types[1] = getRoomType(Vector2(1, 0).add(playerPos));
  types[2] = getRoomType(Vector2(0, -1).add(playerPos));
  types[3] = getRoomType(Vector2(-1, 0).add(playerPos));
}

int Map::indexFromVector2(const Vector2& pos, uint32_t* index) const {
  int32_t x = pos.getX();
  int32_t y = pos.getY();

  if (x >= 0 && x < static_cast<int32_t>(size) &&
      y >= 0 && y < static_cast<int32_t>(size)) {
    uint32_t ux = static_cast<uint32_t>(x);
    uint32_t uy = static_cast<uint32_t>(y);

    *index = ux * size + uy;
    return 0;
  }
  return -1;
}
}  // namespace wumpus
