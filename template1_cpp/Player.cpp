#include "Player.h"

#include <vector>

bool Player::Moved() const
{
  if(coords.x == old_coords.x && coords.y == old_coords.y)
    return false;
  else
    return true;
}

void Player::ProcessInput(MovementDir dir, std::vector<std::vector<char>> &charMap, Action act, Image &background, Image &screen)
{
  if (ChangeLvl)
    return;

  int currTileX = this->coords.x / tileSize;
  int currTileY = this->coords.y / tileSize;
  int addX = this->coords.x % tileSize ? 1 : 0;
  int addY = this->coords.y % tileSize ? 1 : 0;

  if (charMap[currTileX][currTileY] == ' ' || charMap[currTileX + addX][currTileY + addY] == ' ' 
      || charMap[currTileX + addX][currTileY] == ' ' || charMap[currTileX][currTileY + addY] == ' ') {
    this->Dead = true;
    return;
  }

  if (charMap[currTileX][currTileY] == 'x' || charMap[currTileX + addX][currTileY + addY] == 'x' 
      || charMap[currTileX + addX][currTileY] == 'x' || charMap[currTileX][currTileY + addY] == 'x') {
    if (!ChangeLvl)
      this->Level++;
    return;
  }

  if (act == Action::MOVE) {
    if (charMap[currTileX][currTileY] == '#')
      return;
    int move_dist = move_speed * 1;
    switch(dir)
    {
      case MovementDir::UP:
        if (charMap[currTileX][(coords.y + move_dist) / tileSize + addY] == '#')
          return;
        if (charMap[currTileX + addX][(coords.y + move_dist) / tileSize + addY] == '#')
          return;
        old_coords.y = coords.y;
        coords.y += move_dist;
        break;
      case MovementDir::DOWN:
        if (charMap[currTileX][(coords.y - move_dist) / tileSize] == '#')
          return;
        if (charMap[currTileX + addX][(coords.y - move_dist) / tileSize] == '#')
          return;
        old_coords.y = coords.y;
        coords.y -= move_dist;
        break;
      case MovementDir::LEFT:
        if (charMap[(coords.x - move_dist) / tileSize][currTileY] == '#')
          return;
        if (charMap[(coords.x - move_dist) / tileSize][currTileY + addY] == '#')
          return;
        old_coords.x = coords.x;
        coords.x -= move_dist;
        break;
      case MovementDir::RIGHT:
        if (charMap[(coords.x + move_dist) / tileSize + addX][currTileY] == '#')
          return;
        if (charMap[(coords.x + move_dist) / tileSize + addX][currTileY + addY] == '#')
          return;
        old_coords.x = coords.x;
        coords.x += move_dist;
        break;
      default:
        break;
    }
  } else if (act == Action::OPEN) {
    if (charMap[currTileX][currTileY + 1] != 'C')
      return;
    charMap[currTileX][currTileY + 1] = 'O';
    Image ChestOpen("../resources/chest_open.png");
    drawTile(currTileX, currTileY + 1, ChestOpen, background);
    drawTile(currTileX, currTileY + 1, ChestOpen, screen);
  }
}

void Player::Draw(Image &screen, Image &background, MovementDir dir)
{
  Image playerImgFront("../resources/tile007.png");
  Image playerImgLeft("../resources/tile019.png");
  Image playerImgRight("../resources/tile031.png");
  Image playerImgBack("../resources/tile043.png");

  if(Dead) {
    return;
  }

  if(Moved())
  {
    for(int y = old_coords.y; y < old_coords.y + tileSize; ++y)
    {
      for(int x = old_coords.x; x < old_coords.x + tileSize; ++x)
      {
        screen.PutPixel(x, y, background.GetPixel(x, y));
      }
    }
    old_coords = coords;
  }

  for(int y = coords.y + 1; y < coords.y + tileSize; ++y)
  {
    for(int x = coords.x + 1; x < coords.x + tileSize; ++x)
    {
      switch (dir)
      {
      case MovementDir::UP:
        screen.PutPixel(x, y, blend(screen.GetPixel(x, y), playerImgBack.GetPixel(x -  coords.x, tileSize - y + coords.y)));
        break;
      case MovementDir::RIGHT:
        screen.PutPixel(x, y, blend(screen.GetPixel(x, y), playerImgRight.GetPixel(x -  coords.x, tileSize - y + coords.y)));
        break;
      case MovementDir::LEFT:
        screen.PutPixel(x, y, blend(screen.GetPixel(x, y), playerImgLeft.GetPixel(x -  coords.x, tileSize - y + coords.y)));
        break;
      default:
        screen.PutPixel(x, y, blend(screen.GetPixel(x, y), playerImgFront.GetPixel(x -  coords.x, tileSize - y + coords.y)));
        break;
      }
    }
  }
}

void Player::SetStartingPosition(int x, int y) {
  if (x < 0 || y < 0) {
    return;
  }
  this->coords.x = x;
  this->coords.y = y;
}