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
