#include "core.h"

Machine machine; // Если засунуть это в core.h то компилятор будет ругаться на многократное объявление переменной

enum {
  DEFAULT,
  VERBOSE
} loglevel = DEFAULT;
