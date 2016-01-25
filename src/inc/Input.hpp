/** wumpus
 *  Copyright 2016 sixstring982, all rights reserved.
 *
 *  File: Input.hpp
 *  Purpose: Class which gets information from the user via stdin.
 */
#ifndef WUMPUS_SRC_INC_INPUT_HPP_
#define WUMPUS_SRC_INC_INPUT_HPP_

#include <stdint.h>

#include "./Command.hpp"

namespace wumpus {
class Input {
 public:
  uint32_t readLine(char* buffer, uint32_t length) const;

  void pause() const;

  int readUint32(uint32_t* into) const;

  int readCommand(Command* into) const;
};
}  // namespace wumpus

#endif  // WUMPUS_SRC_INC_INPUT_HPP_
