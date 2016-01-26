/** wumpus
 *  Copyright 2016 sixstring982, all rights reserved.
 *
 *  File: TeeMapRenderer.hpp
 *  Purpose: A map renderer which splits its output.
 */
#ifndef WUMPUS_SRC_INC_TEEMAPRENDERER_HPP_
#define WUMPUS_SRC_INC_TEEMAPRENDERER_HPP_

#include "./MapRenderer.hpp"

namespace wumpus {
class TeeMapRenderer : public MapRenderer {
 private:
  MapRenderer* a;
  MapRenderer* b;

 public:
  TeeMapRenderer(MapRenderer* first, MapRenderer* second);

  virtual ~TeeMapRenderer();

  virtual void renderMap(const Map& map, uint32_t points);

  virtual void renderLootResult(LootResult result) const;

  virtual void renderMoveResult(MoveResult result) const;
};
}  // namespace wumpus

#endif  // WUMPUS_SRC_INC_TEEMAPRENDERER_HPP_
