/** wumpus
 *  Copyright 2016 sixstring982, all rights reserved.
 *
 *  File: CLMapRenderer.hpp
 *  Purpose: A class which renders a Wumpus Map to stdout.
 */
#ifndef WUMPUS_SRC_INC_CLMAPRENDERER_HPP_
#define WUMPUS_SRC_INC_CLMAPRENDERER_HPP_

#include "./MapRenderer.hpp"

namespace wumpus {
class CLMapRenderer : public MapRenderer {
 public:
  virtual ~CLMapRenderer();

  virtual void renderMap(const Map& map, uint32_t points);

  virtual void renderLootResult(LootResult result) const;

  virtual void renderMoveResult(MoveResult result) const;

 private:
  char charFromRoomType(RoomType type, bool explored) const;

  void printDescription(RoomType type) const;

  void printClues(const Map& map) const;
};
}  // namespace wumpus

#endif  // WUMPUS_SRC_INC_CLMAPRENDERER_HPP_
