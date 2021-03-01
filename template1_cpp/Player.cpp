#include "Player.h"


bool Player::Moved() const
{
  if(coords.x == old_coords.x && coords.y == old_coords.y)
    return false;
  else
    return true;
}

void Player::ProcessInput(MovementDir dir)
{
  int currTileX = this->coords.x/tileSize;
  int currTileY = this->coords.y/tileSize;
  //if 
  int move_dist = move_speed * 1;
  switch(dir)
  {
    case MovementDir::UP:
      old_coords.y = coords.y;
      coords.y += move_dist;
      break;
    case MovementDir::DOWN:
      old_coords.y = coords.y;
      coords.y -= move_dist;
      break;
    case MovementDir::LEFT:
      old_coords.x = coords.x;
      coords.x -= move_dist;
      break;
    case MovementDir::RIGHT:
      old_coords.x = coords.x;
      coords.x += move_dist;
      break;
    default:
      break;
  }
}

void Player::Draw(Image &screen, Image &background, MovementDir dir)
{
  Image playerImgFront("../resources/tile007.png");
  Image playerImgLeft("../resources/tile019.png");
  Image playerImgRight("../resources/tile031.png");
  Image playerImgBack("../resources/tile043.png");

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

