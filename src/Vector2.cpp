/** wumpus
 *  Copyright 2016 sixstring982, all rights reserved.
 *
 *  File: Vector2.cpp
 *  Purpose: A type for an integral vector with two values.
 */
#include "./inc/Vector2.hpp"

namespace wumpus {
Vector2::Vector2() :
  x(0), y(0) { }

Vector2::Vector2(int32_t xCoord, int32_t yCoord) :
  x(xCoord), y(yCoord) { }

Vector2::Vector2(uint32_t xCoord, uint32_t yCoord) :
  x(static_cast<int32_t>(xCoord)),
  y(static_cast<int32_t>(yCoord)) { }

int32_t Vector2::getX() const {
  return this->x;
}

int32_t Vector2::getY() const {
  return this->y;
}

Vector2& Vector2::add(const Vector2& other) {
  this->x += other.x;
  this->y += other.y;
  return *this;
}

Vector2& Vector2::set(const Vector2& other) {
  this->x = other.x;
  this->y = other.y;
  return *this;
}

Vector2& Vector2::set(int32_t xCoord, int32_t yCoord) {
  this->x = xCoord;
  this->y = yCoord;
  return *this;
}

Vector2& Vector2::set(uint32_t xCoord, uint32_t yCoord) {
  this->x = static_cast<int32_t>(xCoord);
  this->y = static_cast<int32_t>(yCoord);
  return *this;
}
}  // namespace wumpus
