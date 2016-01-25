/** wumpus
 *  Copyright 2016 sixstring982, all rights reserved.
 *
 *  File: Input.cpp
 *  Purpose: Class which gets information from the user via stdin.
 */
#include "./inc/Input.hpp"

#include <ctype.h>  /* For isdigit */
#include <stdio.h>  /* For fgetc, EOF */
#include <stdlib.h> /* For atoi */

namespace wumpus {
uint32_t Input::readLine(char* buffer, uint32_t length) const {
  uint32_t i;
  int readC;

  /* Read a line of input */
  for (i = 0; i < length - 1; i++) {
    readC = fgetc(stdin);
    if (readC == '\n' ||
        readC == EOF) {
      break;
    } else {
      buffer[i] = static_cast<char>(readC);
    }
  }
  buffer[i] = '\0';

  return i;
}

void Input::pause() const {
  int readC;
  do {
    readC = fgetc(stdin);
  } while (readC != EOF && readC != '\n');
}

int Input::readUint32(uint32_t* into) const {
  const uint32_t BUFFER_SIZE = 256;
  char buffer[BUFFER_SIZE];

  readLine(buffer, BUFFER_SIZE);

  /* Empty strings won't work */
  if (*buffer == '\0') {
    return -1;
  }

  /* Verify that this is only positive digits */
  for (size_t i = 0; i < BUFFER_SIZE; i++) {
    if (buffer[i] == '\0') {
      break;
    } else if (!isdigit(buffer[i])) {
      return -1;
    }
  }

  /* Convert to int and store. */
  *into = static_cast<uint32_t>(atoi(buffer));
  return 0;
}

static int commandFromChar(char c, Command* into) {
  switch (c) {
  case '?': *into = COMMAND_LIST; return 0;
  case 'N': *into = COMMAND_NORTH; return 0;
  case 'S': *into = COMMAND_SOUTH; return 0;
  case 'E': *into = COMMAND_EAST; return 0;
  case 'W': *into = COMMAND_WEST; return 0;
  case 'L': *into = COMMAND_LOOT; return 0;
  case 'R': *into = COMMAND_RUN; return 0;
  case 'X': *into = COMMAND_HALT; return 0;
  default: return -1;
  }
}

int Input::readCommand(Command* into) const {
  const uint32_t BUFFER_SIZE = 256;
  char buffer[BUFFER_SIZE];

  uint32_t charsRead = readLine(buffer, BUFFER_SIZE);

  if (charsRead != 1) {
    return -1;
  } else {
    return commandFromChar(*buffer, into);
  }
}
}  // namespace wumpus
