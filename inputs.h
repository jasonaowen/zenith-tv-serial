#ifndef INPUTS_H
#define INPUTS_H

#define INPUT_TABLE \
X(DTV, 0)           \
X(ANALOG, 1)        \
X(AV1, 2)           \
X(AV2, 3)           \
X(COMP1, 4)         \
X(COMP2, 5)         \
X(PC, 7)            \
X(HDMI1, 8)         \
X(HDMI2, 9)

#define X(name, id) name,
typedef enum {
  INPUT_TABLE
  INPUT_COUNT
} INPUT;
#undef X

extern const char INPUT_NAMES[][INPUT_COUNT];
extern const int INPUT_IDS[];
#endif
