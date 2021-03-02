#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"

struct Point
{
  int x;
  int y;
};

enum class MovementDir
{
  UP,
  DOWN,
  LEFT,
  RIGHT
};

enum Action
{
  MOVE,
  OPEN
};


struct Player
{
  explicit Player(Point pos = {.x = 10, .y = 10}) :
                 coords(pos), old_coords(coords) {};

  bool Moved() const;
  bool Active = false;
  bool Dead = false;
  void ProcessInput(MovementDir dir, std::vector<std::vector<char>> &charMap, Action act, Image &background);
  void Draw(Image &screen, Image &background, MovementDir dir);

private:
  Point coords {.x = 10, .y = 10};
  Point old_coords {.x = 10, .y = 10};
  Pixel color {.r = 255, .g = 0, .b = 255, .a = 255};
  int move_speed = 4;

};

#endif //MAIN_PLAYER_H
