#include <stdbool.h>
#include <stdio.h>

typedef enum {
  Unexplored,
  Cross,
  T,
  Right,
  Left,
  Straight,
  DeadEnd,
  End,
} Intersection_t;

typedef enum {
  R,    // Right
  L,    // Left
  S,    // Straight
  U,    // U-turn
  Stop, // Stop the bot as we have reached the end
} Turn_t;

// TODO: Implement the baising
// // Prefer to go straight when possible
// Turn_t bias = S;

typedef enum {
  North = 0,
  East,
  South,
  West,
} Direction_t;
// Starting direction can be considered north
Direction_t curr_dir = North;

typedef struct {
  Direction_t dir;
  Intersection_t intersection;
  bool north_explored;
  bool east_explored;
  bool south_explored;
  bool west_explored;
  int times_visited;
  // int gradient; // TODO(optimization)
} Map;

// In memory representation of the actual map that has been traversed
Map map[5][5];

void move_bot(Turn_t turn);
void update_dir(Turn_t turn) {
  switch (turn) {
  case R:
    curr_dir += 1;
    curr_dir %= 4;
    break;
  case L:
    curr_dir -= 1;
    curr_dir %= 4;
    break;
  case S:
    break;
  case U:
    curr_dir += 2;
    curr_dir %= 4;
    break;
  case Stop:
    break;
  };
};

// NOTE: We are currently biasing the straight direction followed by left and
// then right
Turn_t handle_cross_intersection(int x, int y, Map *curr_tile) {
  switch (curr_dir) {
  case North:
    if (!curr_tile->north_explored) { // Straight
      return S;
    } else if (!curr_tile->west_explored) { // Left
      return L;
    } else if (!curr_tile->east_explored) { // Right
      return R;
    }
  case East:
    if (!curr_tile->east_explored) { // Straight
      return S;
    } else if (!curr_tile->south_explored) { // Left
      return L;
    } else if (!curr_tile->north_explored) { // Right
      return R;
    }
  case South:
    if (!curr_tile->south_explored) { // Straight
      return S;
    } else if (!curr_tile->east_explored) { // Left
      return L;
    } else if (!curr_tile->west_explored) { // Right
      return R;
    }
  case West:
    if (!curr_tile->west_explored) { // Straight
      return S;
    } else if (!curr_tile->north_explored) { // Left
      return L;
    } else if (!curr_tile->south_explored) { // Right
      return R;
    }
  }

  // NOTE: We shouldn't reach this point ever!
  return S;
}

Turn_t handle_intersection_type(int x, int y, Intersection_t intersection) {
  Map *curr_tile = &map[y][x];
  if (map[y][x].intersection == Unexplored) {
    curr_tile->dir = curr_dir;
    curr_tile->intersection = intersection;
  }
  curr_tile->times_visited++;

  // Update the tile to store that we have visitied the direction we are coming
  // from!
  switch (curr_dir) {
  case North:
    curr_tile->south_explored = true;
    break;
  case East:
    curr_tile->west_explored = true;
    break;
  case South:
    curr_tile->north_explored = true;
    break;
  case West:
    curr_tile->east_explored = true;
    break;
  }

  switch (intersection) {
  case Cross:
    return handle_cross_intersection(x, y, curr_tile);
  case T:
    break;
  case Right:
    break;
  case Left:
    break;
  case Straight:
    break;
  case DeadEnd:
    return U;
  case End:
    return Stop;

  // This case should never be reachable
  case Unexplored:
    break;
  }

  return 0;
}

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
