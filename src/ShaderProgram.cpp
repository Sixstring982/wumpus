/** wumpus
 *  Copyright 2016 sixstring982, all rights reserved.
 *
 *  File: ShaderProgram.cpp
 *  Purpose: An abstraction for loading OpenGL Shader programs
 */
#include "./inc/ShaderProgram.hpp"

#include <stdio.h>
#include <stdlib.h>

namespace wumpus {
ShaderProgram::ShaderProgram() :
  programId(0), vertexId(0), fragmentId(0), ready(false) { }

static int mallocFileContents(const char* filename, char** outBuffer) {
  const uint32_t INITIAL_SIZE = 64;
  uint32_t bufferCapacity = INITIAL_SIZE;
  char* buffer = reinterpret_cast<char*>(malloc(sizeof(char) * bufferCapacity));
  FILE* fp = fopen(filename, "r");
  int readC;
  uint32_t charsRead = 0;

  if (fp == NULL) {
    free(buffer);
    return -1;
  }

  while ((readC = fgetc(fp)) != EOF) {
    buffer[charsRead++] = static_cast<char>(readC);
    if (charsRead >= bufferCapacity) {
      buffer = reinterpret_cast<char*>(realloc(
                  reinterpret_cast<void*>(buffer), bufferCapacity * 2));
      if (buffer != NULL) {
        bufferCapacity *= 2;
      } else {
        fclose(fp);
        return -1;
      }
    }
  }

  buffer[charsRead] = '\0';
  *outBuffer = buffer;
  fclose(fp);
  return 0;
}

static void printLog(GLuint shader) {
  GLint logLength;
  GLchar* log;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
  log = reinterpret_cast<GLchar*>(malloc(sizeof(GLchar) *
                                         static_cast<uint32_t>(logLength)));
  glGetShaderInfoLog(shader, logLength, &logLength, log);
  fprintf(stderr, "%s\n", reinterpret_cast<char*>(log));

  free(log);
}

int ShaderProgram::load(const char* vertexFilename,
                    const char* fragmentFilename) {
  char* vertexSource;
  char* fragmentSource;

  /* Create the program */
  if ((programId = glCreateProgram()) == 0) {
    return -1;
  }

  /* Create both shaders */
  if ((vertexId = glCreateShader(GL_VERTEX_SHADER)) == 0) {
    glDeleteProgram(programId);
    return -1;
  }
  if ((fragmentId = glCreateShader(GL_FRAGMENT_SHADER)) == 0) {
    glDeleteProgram(programId);
    glDeleteShader(vertexId);
    return -1;
  }

  /* Read the source */
  if (mallocFileContents(vertexFilename, &vertexSource) == -1) {
    glDeleteProgram(programId);
    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);
    return -1;
  } else if (mallocFileContents(fragmentFilename, &fragmentSource) == -1) {
    glDeleteProgram(programId);
    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);
    free(vertexSource);
    return -1;
  }

  const GLchar** glVertexSource = const_cast<const GLchar**>(&vertexSource);
  const GLchar** glFragmentSource = const_cast<const GLchar**>(&fragmentSource);

  /* Compile both programs */
  GLint status;
  glShaderSource(vertexId, 1, glVertexSource, NULL);
  glCompileShader(vertexId);
  glGetShaderiv(vertexId, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    fprintf(stderr, "Vertex Shader compile error.\n");
    printLog(vertexId);
    glDeleteProgram(programId);
    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);
    free(vertexSource);
    free(fragmentSource);
    return -1;
  }

  glShaderSource(fragmentId, 1, glFragmentSource, NULL);
  glCompileShader(fragmentId);
  glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    fprintf(stderr, "Fragment Shader compile error.\n");
    printLog(fragmentId);
    glDeleteProgram(programId);
    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);
    free(vertexSource);
    free(fragmentSource);
    return -1;
  }

  glAttachShader(programId, vertexId);
  glAttachShader(programId, fragmentId);
  glLinkProgram(programId);

  glGetProgramiv(programId, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    fprintf(stderr, "Shader link error.\n");
    return -1;
  }

  free(vertexSource);
  free(fragmentSource);
  ready = true;
  return 0;
}

int ShaderProgram::unload() {
  if (!isReady()) {
    return -1;
  } else {
    glDeleteProgram(programId);
    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);
    return 0;
  }
}

bool ShaderProgram::isReady() const {
  return ready;
}

int ShaderProgram::use() const {
  if (!isReady()) {
    return -1;
  } else {
    glUseProgram(programId);
    return 0;
  }
}

int ShaderProgram::setVector2Uniform(const char* name,
                                     const Vector2& v) const {
  if (!isReady()) {
    return -1;
  } else {
    GLint loc = glGetUniformLocation(programId, name);
    if (loc == -1) {
      return -1;
    } else {
      glUniform2f(loc, v.getX(), v.getY());
      return 0;
    }
  }
}

int ShaderProgram::setFloatUniform(const char* name,
                                   float value) const {
  if (!isReady()) {
    return -1;
  } else {
    GLint loc = glGetUniformLocation(programId, name);
    if (loc == -1) {
      return -1;
    } else {
      glUniform1f(loc, value);
      return 0;
    }
  }
}
}  // namespace wumpus
