/** wumpus
 *  Copyright 2016 sixstring982, all rights reserved.
 *
 *  File: RoomType.hpp
 *  Purpose: An enum describing the types of rooms.
 */
#ifndef WUMPUS_SRC_INC_ROOMTYPE_HPP_
#define WUMPUS_SRC_INC_ROOMTYPE_HPP_

namespace wumpus {
enum RoomType {
  ROOM_ENTRANCE,
  ROOM_WUMPUS,
  ROOM_PIT,
  ROOM_GOLD,
  ROOM_WEAPON,
  ROOM_EMPTY,
  ROOM_WALL
};
}  // namespace wumpus

#endif  // WUMPUS_SRC_INC_ROOMTYPE_HPP_
