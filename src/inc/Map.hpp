/** wumpus
 *  Copyright 2016 sixstring982, all rights reserved.
 *
 *  File: Map.hpp
 *  Purpose: The Map that will be explored in the Wumpus game.
 */
#ifndef WUMPUS_SRC_INC_MAP_HPP_
#define WUMPUS_SRC_INC_MAP_HPP_

#include "./RoomType.hpp"
#include "./Vector2.hpp"

namespace wumpus {
class Map {
 private:
  static const uint32_t MAX_SIZE;
  bool* explored;
  RoomType* rooms;
  Vector2 playerPos;
  uint32_t size;

 public:
  Map();

  ~Map();

  RoomType getRoomType(const Vector2& pos) const;

  bool getHasBeenExplored(const Vector2& pos) const;

  void fillPlayerPosition(Vector2* into) const;

  int movePlayerToEntrance();

  int movePlayer(const Vector2& offset);

  int setup(uint32_t size);

  void fillAdjacentRooms(RoomType* types) const;

 private:
  void layoutRooms();

  void setRoom(const Vector2& pos, RoomType type);

  void convertEmptyRooms(unsigned int* seed, RoomType type, uint32_t count);

  int indexFromVector2(const Vector2& pos, uint32_t* index) const;
};
}  // namespace wumpus

#endif  // WUMPUS_SRC_INC_MAP_HPP_
