/** wumpus
 *  Copyright 2016 sixstring982, all rights reserved.
 *
 *  File: Command.hpp
 *  Purpose: An enumeration of the commands that can be entered by a
 *           user.
 */
#ifndef WUMPUS_SRC_INC_COMMAND_HPP_
#define WUMPUS_SRC_INC_COMMAND_HPP_

namespace wumpus {
enum Command {
  COMMAND_LIST,
  COMMAND_NORTH,
  COMMAND_EAST,
  COMMAND_SOUTH,
  COMMAND_WEST,
  COMMAND_LOOT,
  COMMAND_RUN,
  COMMAND_HALT
};
}  // namespace wumpus

#endif  // WUMPUS_SRC_INC_COMMAND_HPP_
