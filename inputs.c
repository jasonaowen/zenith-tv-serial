#include "inputs.h"

#define X(name, id) #name,
const char INPUT_NAMES[][INPUT_COUNT] = {
  INPUT_TABLE
};
#undef X

#define X(name, id) id,
const int INPUT_IDS[] = {
  INPUT_TABLE
};

#undef X
