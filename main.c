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

void rotate_bot(Turn_t turn); // Rotates the bot according to the given turn.
void move_bot();              // Moves the bot ahead by 1 unit.

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

Turn_t handle_cross_intersection(Map *curr_tile) {
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

Turn_t handle_T_intersection(Map *curr_tile) {
  switch (curr_tile->dir) {
  case North:
    switch (curr_dir) {
    case North:
      if (!curr_tile->west_explored) {
        return L;
      } else {
        return R;
      }
    case East:
      if (!curr_tile->east_explored) {
        return S;
      } else {
        return R;
      }
    case West:
      if (!curr_tile->west_explored) {
        return S;
      } else {
        return L;
      }
    // This case should not be possible!
    case South:
      return S;
    }
  case East:
    switch (curr_dir) {
    case North:
      if (!curr_tile->north_explored) {
        return S;
      } else {
        return L;
      }
    case East:
      if (!curr_tile->north_explored) {
        return L;
      } else {
        return R;
      }
    // This case should not be possible!
    case West:
      return S;
    case South:
      if (!curr_tile->south_explored) {
        return S;
      } else {
        return R;
      }
    }
  case South:
    switch (curr_dir) {
    // This case should not be possible!
    case North:
      return S;
    case East:
      if (!curr_tile->east_explored) {
        return S;
      } else {
        return L;
      }
    case West:
      if (!curr_tile->west_explored) {
        return S;
      } else {
        return R;
      }
    case South:
      if (!curr_tile->east_explored) {
        return L;
      } else {
        return R;
      }
    }
  case West:
    switch (curr_dir) {
    case North:
      if (!curr_tile->north_explored) {
        return S;
      } else {
        return R;
      }
    // This case should not be possible!
    case East:
      return S;
    case West:
      if (!curr_tile->south_explored) {
        return L;
      } else {
        return R;
      }
    case South:
      if (!curr_tile->south_explored) {
        return S;
      } else {
        return L;
      }
    }
  }
}

Turn_t handle_rl_intersection(Map *curr_tile, Turn_t turn1, Turn_t turn2) {
  switch (curr_tile->dir) {
  case North:
    switch (curr_dir) {
    case North:
      return turn1;
    case West:
      return turn2;
    default:
      return S;
    }
  case East:
    switch (curr_dir) {
    case East:
      return turn1;
    case North:
      return turn2;
    default:
      return S;
    }
  case South:
    switch (curr_dir) {
    case South:
      return turn1;
    case East:
      return turn2;
    default:
      return S;
    }
  case West:
    switch (curr_dir) {
    case West:
      return turn1;
    case South:
      return turn2;
    default:
      return S;
    }
  }
}

Turn_t handle_intersection_type(int x, int y, Intersection_t intersection) {
  Map *curr_tile = &map[y][x];
  if (map[y][x].intersection == Unexplored) {
    curr_tile->dir = curr_dir;
    curr_tile->intersection = intersection;
  }
  curr_tile->times_visited++;

  // Update the tile to store that we have visitied the direction we are
  // coming from!
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
    return handle_cross_intersection(curr_tile);
  case T:
    return handle_T_intersection(curr_tile);
  case Right:
    return handle_rl_intersection(curr_tile, R, L);
  case Left:
    return handle_rl_intersection(curr_tile, L, R);
  case Straight:
    return S;
  case DeadEnd:
    return U;
  case End:
    return Stop;

  // This case should never be reachable
  case Unexplored:
    break;
  }

  // This case should never be reachable
  return 0;
}

void update_map_location(int *x, int *y) {
  switch (curr_dir) {
  case North:
    (*y)++;
    break;
  case East:
    (*x)--;
    break;
  case South:
    (*y)--;
    break;
  case West:
    (*x)++;
    break;
  }
}

int main() {
  int x = 0, y = 0;
  int nextStep;

  while (true) {
    // TODO: Get the next intersection type from the sensors!
    printf("Enter next intersection type: ");
    scanf("%d", &nextStep);
    Turn_t turn = handle_intersection_type(x, y, (Intersection_t)nextStep);
    if (turn == Stop) {
      break;
    }

    update_dir(turn);
    update_map_location(&x, &y);
    printf("Updating...%d, %d, %d, %d\n", turn, curr_dir, x, y);
  }

  printf("Maze completed!\nFinal Position: %d, %d\n", x, y);

  // for (int i = 0; i < sizeof(map) / sizeof(int); i++) {
  //   for (int j = 0; i < sizeof(map) / sizeof(int); i++) {
  //     Map tile = map[i][j];
  //     printf(
  //         "dir: %d\tint: %d\tnorth: %d\teast: %d\tsouth: %d\twest: %d\ttimes:
  //         "
  //         "%d\n",
  //         tile.dir, tile.intersection, tile.north_explored,
  //         tile.east_explored, tile.south_explored, tile.east_explored,
  //         tile.times_visited);
  //   }
  // }

  return 0;
}
