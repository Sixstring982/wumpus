/** wumpus
 *  Copyright 2016 sixstring982, all rights reserved.
 *
 *  File: MapRenderer.hpp
 *  Purpose: An interface which renders a Wumpus Map.
 */
#ifndef WUMPUS_SRC_INC_MAPRENDERER_HPP_
#define WUMPUS_SRC_INC_MAPRENDERER_HPP_

#include "./MoveResult.hpp"
#include "./LootResult.hpp"
#include "./Map.hpp"

namespace wumpus {
class MapRenderer {
 public:
  virtual void renderMap(const Map& map, uint32_t points) = 0;

  virtual void renderLootResult(LootResult result) const = 0;

  virtual void renderMoveResult(MoveResult result) const = 0;

 protected:
  ~MapRenderer();
};
}  // namespace wumpus

#endif  // WUMPUS_SRC_INC_MAPRENDERER_HPP_
