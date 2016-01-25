/** wumpus
 *  Copyright 2016 sixstring982, all rights reserved.
 *
 *  File: Map.hpp
 *  Purpose: The Map that will be explored in the Wumpus game.
 */
#ifndef WUMPUS_SRC_INC_MAP_HPP_
#define WUMPUS_SRC_INC_MAP_HPP_

#include "./Direction.hpp"
#include "./LootResult.hpp"
#include "./MoveResult.hpp"
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
  bool armed;

 public:
  Map();

  ~Map();

  RoomType getRoomType(const Vector2& pos) const;

  bool getHasBeenExplored(const Vector2& pos) const;

  bool isArmed() const;

  bool isAtExit() const;

  void fillPlayerPosition(Vector2* into) const;

  int movePlayerToEntrance();

  MoveResult movePlayer(Direction dir);

  int setup(uint32_t size);

  void fillAdjacentRooms(RoomType* types) const;

  LootResult loot();

 private:
  void layoutRooms();

  void setRoom(const Vector2& pos, RoomType type);

  void convertEmptyRooms(unsigned int* seed, RoomType type, uint32_t count);

  void convertAll(RoomType from, RoomType to);

  void convertWeaponsToGold();

  void removeWumpus();

  void setExplored(const Vector2& pos);

  int indexFromVector2(const Vector2& pos, uint32_t* index) const;
};
}  // namespace wumpus

#endif  // WUMPUS_SRC_INC_MAP_HPP_
