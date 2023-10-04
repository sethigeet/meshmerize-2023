#include <stdbool.h>
#include <stdio.h>

typedef enum {
  Cross,
  T,
  Right,
  Left,
  Straight,
  DeadEnd,
  End,
} Intersection_t;

typedef enum {
  R,
  L,
  S,
  U,
} Turn_t;

// Prefer to go straight when possible
Turn_t bias = S;

typedef enum {
  North = 0,
  East,
  South,
  West,
} Direction_t;
// Starting direction can be considered north
Direction_t currDir = North;

typedef struct {
  Direction_t dir;
  Intersection_t intersection;
  bool rightExplored;
  bool leftExplored;
  bool straightExplored;
  // int gradient; // TODO(optimization)
} Map;

// In memory representation of the actual map that has been traversed
Map map[20][20];

void move_bot(Turn_t turn);
void update_dir(Turn_t turn) {
  switch (turn) {
  case R:
    currDir += 1;
    currDir %= 4;
    break;
  case L:
    currDir -= 1;
    currDir %= 4;
    break;
  case S:
    break;
  case U:
    currDir += 2;
    currDir %= 4;
    break;
  };
};

int main() {
  int x, y = 0;
  int nextStep;

  while (map[y][x].intersection != End) {
    // TODO: Get the next intersection type from the sensors!
    scanf("Enter next intersetion type: %d", &nextStep);
    // TODO: Do something with the input!
  }

  return 0;
}
