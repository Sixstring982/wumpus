/** wumpus
 *  Copyright 2016 sixstring982, all rights reserved.
 *
 *  File: Vector2.hpp
 *  Purpose: A type for an integral vector with two values.
 */
#ifndef WUMPUS_SRC_INC_VECTOR2_HPP_
#define WUMPUS_SRC_INC_VECTOR2_HPP_

#include <stdint.h>

namespace wumpus {
class Vector2 {
 private:
  int32_t x;
  int32_t y;

 public:
  Vector2();

  Vector2(int32_t x, int32_t y);

  Vector2(uint32_t x, uint32_t y);

  int32_t getX() const;

  int32_t getY() const;

  Vector2& add(const Vector2& other);

  Vector2& set(const Vector2& other);

  Vector2& set(int32_t x, int32_t y);

  Vector2& set(uint32_t x, uint32_t y);
};
}  // namespace wumpus

#endif  // WUMPUS_SRC_INC_VECTOR2_HPP_
