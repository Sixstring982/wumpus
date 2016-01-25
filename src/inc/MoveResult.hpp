/** wumpus
 *  Copyright 2016 sixstring982, all rights reserved.
 *
 *  File: MoveResult.cpp
 *  Purpose: Enum for differerent moveing results.
 */
#ifndef WUMPUS_SRC_INC_MOVERESULT_HPP_
#define WUMPUS_SRC_INC_MOVERESULT_HPP_

namespace wumpus {
enum MoveResult {
  MOVE_DISCOVER,
  MOVE_DIE_PIT,
  MOVE_DIE_WUMPUS,
  MOVE_KILL_WUMPUS,
  MOVE_ILLEGAL,
  MOVE_UNEVENTFUL
};
}  // namespace wumpus

#endif  // WUMPUS_SRC_INC_MOVERESULT_HPP_
