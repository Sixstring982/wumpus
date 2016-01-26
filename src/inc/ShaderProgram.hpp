/** wumpus
 *  Copyright 2016 sixstring982, all rights reserved.
 *
 *  File: ShaderProgram.hpp
 *  Purpose: An abstraction for loading OpenGL Shader programs
 */
#ifndef WUMPUS_SRC_INC_SHADERPROGRAM_HPP_
#define WUMPUS_SRC_INC_SHADERPROGRAM_HPP_

#include "./OpenGL.hpp"
#include "./Vector2.hpp"

namespace wumpus {
class ShaderProgram {
 private:
  GLuint programId;
  GLuint vertexId;
  GLuint fragmentId;
  bool ready;

 public:
  ShaderProgram();

  int load(const char* vertexFilename,
           const char* fragmentFilename);

  int unload();

  bool isReady() const;

  int use() const;

  int setVector2Uniform(const char* name,
                        const Vector2& v) const;

  int setFloatUniform(const char* name,
                      float value) const;
};
}  // namespace wumpus

#endif  // WUMPUS_SRC_INC_SHADERPROGRAM_HPP_
