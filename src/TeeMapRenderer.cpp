/** wumpus
 *  Copyright 2016 sixstring982, all rights reserved.
 *
 *  File: TeeMapRenderer.cpp
 *  Purpose: A map renderer which splits its output.
 */
#include "./inc/TeeMapRenderer.hpp"

namespace wumpus {
TeeMapRenderer::TeeMapRenderer(MapRenderer* first, MapRenderer* second) {
  this->a = first;
  this->b = second;
}

TeeMapRenderer::~TeeMapRenderer() { }

void TeeMapRenderer::renderMap(const Map& map, uint32_t points) {
  a->renderMap(map, points);
  b->renderMap(map, points);
}

void TeeMapRenderer::renderLootResult(LootResult result) const {
  a->renderLootResult(result);
  b->renderLootResult(result);
}

void TeeMapRenderer::renderMoveResult(MoveResult result) const {
  a->renderMoveResult(result);
  b->renderMoveResult(result);
}
}  // namespace wumpus
