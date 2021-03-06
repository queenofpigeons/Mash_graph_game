#include "Player.h"

#include <vector>

bool Player::Moved() const
{
    int delta_x = coords.x - old_coords.x;
    int delta_y = coords.y - old_coords.y;

    if (delta_x == 0 && delta_y == 0) {
        return false;
    }
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
        if (charMap[currTileX][(coords.y + move_dist) / tileSize + addY] == '#' ||
              charMap[currTileX][(coords.y + move_dist) / tileSize + addY] == 'D')
          return;
        if (charMap[currTileX + addX][(coords.y + move_dist) / tileSize + addY] == '#' ||
              charMap[currTileX + addX][(coords.y + move_dist) / tileSize + addY] == 'D')
          return;
        old_coords.y = coords.y;
        coords.y += move_dist;
        break;
      case MovementDir::DOWN:
        if (charMap[currTileX][(coords.y - move_dist) / tileSize] == '#' ||
              charMap[currTileX][(coords.y - move_dist) / tileSize] == 'D')
          return;
        if (charMap[currTileX + addX][(coords.y - move_dist) / tileSize] == '#' ||
              charMap[currTileX + addX][(coords.y - move_dist) / tileSize] == 'D')
          return;
        old_coords.y = coords.y;
        coords.y -= move_dist;
        break;
      case MovementDir::LEFT:
        if (charMap[(coords.x - move_dist) / tileSize][currTileY] == '#' ||
              charMap[(coords.x - move_dist) / tileSize][currTileY] == 'D')
          return;
        if (charMap[(coords.x - move_dist) / tileSize][currTileY + addY] == '#' ||
              charMap[(coords.x - move_dist) / tileSize][currTileY + addY] == 'D')
          return;
        old_coords.x = coords.x;
        coords.x -= move_dist;
        break;
      case MovementDir::RIGHT:
        if (charMap[(coords.x + move_dist) / tileSize + addX][currTileY] == '#' ||
              charMap[(coords.x + move_dist) / tileSize + addX][currTileY] == 'D')
          return;
        if (charMap[(coords.x + move_dist) / tileSize + addX][currTileY + addY] == '#' ||
              charMap[(coords.x + move_dist) / tileSize + addX][currTileY + addY] == 'D')
          return;
        old_coords.x = coords.x;
        coords.x += move_dist;
        break;
      default:
        break;
    }
  } else if (act == Action::OPEN) {
    Image Door("../resources/door.png");
    Image DoorOpen("../resources/door_open.png");
    Image ChestOpen("../resources/chest_open.png");

    switch (dir) {
      case MovementDir::UP:
        if (charMap[currTileX][currTileY + 1] == 'C') {
          charMap[currTileX][currTileY + 1] = 'O';
          drawTile(currTileX, currTileY + 1, ChestOpen, background);
          drawTile(currTileX, currTileY + 1, ChestOpen, screen);
        } else if (charMap[currTileX][currTileY + addY + 1] == 'D') {
          charMap[currTileX][currTileY + addY + 1] = 'd';
          drawTile(currTileX, currTileY + addY + 1, DoorOpen, background);
          drawTile(currTileX, currTileY + addY + 1, DoorOpen, screen);
        } else if (charMap[currTileX][currTileY + addY + 1] == 'd') {
          charMap[currTileX][currTileY + addY + 1] = 'D';
          drawTile(currTileX, currTileY + addY + 1, Door, background);
          drawTile(currTileX, currTileY + addY + 1, Door, screen);
        }
        break;
      case MovementDir::DOWN:
        if (charMap[currTileX][currTileY - 1] == 'D') {
          charMap[currTileX][currTileY - 1] = 'd';
          drawTile(currTileX, currTileY - 1, DoorOpen, background);
          drawTile(currTileX, currTileY - 1, DoorOpen, screen);
        } else if (charMap[currTileX][currTileY - 1] == 'd') {
          charMap[currTileX][currTileY - 1] = 'D';
          drawTile(currTileX, currTileY - 1, Door, background);
          drawTile(currTileX, currTileY - 1, Door, screen);
        }
      case MovementDir::RIGHT:
        if (charMap[currTileX + addX + 1][currTileY] == 'D') {
          charMap[currTileX + addX + 1][currTileY] = 'd';
          drawTile(currTileX + addX + 1, currTileY, DoorOpen, background);
          drawTile(currTileX + addX + 1, currTileY, DoorOpen, screen);
        } else if (charMap[currTileX + addX + 1][currTileY] == 'd') {
          charMap[currTileX + addX + 1][currTileY] = 'D';
          drawTile(currTileX + addX + 1, currTileY, Door, background);
          drawTile(currTileX + addX + 1, currTileY, Door, screen);
        }
      case MovementDir::LEFT:
        if (charMap[currTileX - 1][currTileY] == 'D') {
          charMap[currTileX - 1][currTileY] = 'd';
          drawTile(currTileX - 1, currTileY, DoorOpen, background);
          drawTile(currTileX - 1, currTileY, DoorOpen, screen);
        } else if (charMap[currTileX - 1][currTileY] == 'd') {
          charMap[currTileX - 1][currTileY] = 'D';
          drawTile(currTileX - 1, currTileY, Door, background);
          drawTile(currTileX - 1, currTileY, Door, screen);
        }
      default:
        break;
    }
  }
}

void Player::Draw(Image &screen, Image &background, MovementDir dir)
{
  if(Dead) {
    return;
  }

  std::vector<Image*> front(frames);
  front[0] = new Image("../resources/tile006.png");
  front[1] = new Image("../resources/tile007.png");
  front[2] = new Image("../resources/tile008.png");

  std::vector<Image*> left(frames);
  left[0] = new Image("../resources/tile018.png");
  left[1] = new Image("../resources/tile019.png");
  left[2] = new Image("../resources/tile019.png");

  std::vector<Image*> right(frames);
  right[0] = new Image("../resources/tile030.png");
  right[1] = new Image("../resources/tile031.png");
  right[2] = new Image("../resources/tile032.png");

  std::vector<Image*> back(frames);
  back[0] = new Image("../resources/tile042.png");
  back[1] = new Image("../resources/tile043.png");
  back[2] = new Image("../resources/tile044.png");

  if(Moved())
  {
    for(int y = old_coords.y; y < old_coords.y + tileSize; ++y)
    {
      for(int x = old_coords.x; x < old_coords.x + tileSize; ++x)
      {
        screen.PutPixel(x, y, background.GetPixel(x, y));
      }
    }
  }

  for(int y = coords.y + 1; y < coords.y + tileSize; ++y)
  {
    for(int x = coords.x + 1; x < coords.x + tileSize; ++x)
    {
      if(Moved()) {
        this->frame++;
        this->frame %= frame_max;
      }
      switch (dir)
      {
      case MovementDir::UP:
        screen.PutPixel(x, y, blend(screen.GetPixel(x, y), back[this->frame / frame_divider]->GetPixel(x -  coords.x, tileSize - y + coords.y)));
        break;
      case MovementDir::RIGHT:
        screen.PutPixel(x, y, blend(screen.GetPixel(x, y), right[this->frame / frame_divider]->GetPixel(x -  coords.x, tileSize - y + coords.y)));
        break;
      case MovementDir::LEFT:
        screen.PutPixel(x, y, blend(screen.GetPixel(x, y), left[this->frame / frame_divider]->GetPixel(x -  coords.x, tileSize - y + coords.y)));
        break;
      default:
        screen.PutPixel(x, y, blend(screen.GetPixel(x, y), front[this->frame / frame_divider]->GetPixel(x -  coords.x, tileSize - y + coords.y)));
        break;
      }
    }
  }

  if (Moved()) {
    old_coords = coords;
  }

  delete front[0];
  delete front[1];
  delete front[2];

  delete right[0];
  delete right[1];
  delete right[2];
  
  delete left[0];
  delete left[1];
  delete left[2];

  delete back[0];
  delete back[1];
  delete back[2];
}

void Player::SetStartingPosition(int x, int y) {
  if (x < 0 || y < 0) {
    return;
  }
  this->coords.x = x;
  this->coords.y = y;
}