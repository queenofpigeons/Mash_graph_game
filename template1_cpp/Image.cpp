#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>
#include <fstream>

Pixel blend(Pixel oldPixel, Pixel newPixel)
{
  newPixel.r = newPixel.a / 255.0 * (newPixel.r - oldPixel.r) + oldPixel.r;
  newPixel.g = newPixel.a / 255.0 * (newPixel.g - oldPixel.g) + oldPixel.g;
  newPixel.b = newPixel.a / 255.0 * (newPixel.b - oldPixel.b) + oldPixel.b;
  newPixel.a = 255;

  return newPixel;
}

//TO-DO
Pixel blendScreen(Pixel oldPixel, Pixel newPixel)
{
  newPixel.r = newPixel.a / 255.0 * (newPixel.r - oldPixel.r) + oldPixel.r;
  newPixel.g = newPixel.a / 255.0 * (newPixel.g - oldPixel.g) + oldPixel.g;
  newPixel.b = newPixel.a / 255.0 * (newPixel.b - oldPixel.b) + oldPixel.b;
  newPixel.a = 255;

  return newPixel;
}

Image::Image(const std::string &a_path)
{
  if((data = (Pixel*)stbi_load(a_path.c_str(), &width, &height, &channels, 4)) != nullptr)
  {
    size = width * height * channels;
  }
}

Image::Image(int a_width, int a_height, int a_channels)
{
  data = new Pixel[a_width * a_height ]{};

  if(data != nullptr)
  {
    width = a_width;
    height = a_height;
    size = a_width * a_height * a_channels;
    channels = a_channels;
    self_allocated = true;
  }
}


int Image::Save(const std::string &a_path)
{
  auto extPos = a_path.find_last_of('.');
  if(a_path.substr(extPos, std::string::npos) == ".png" || a_path.substr(extPos, std::string::npos) == ".PNG")
  {
    stbi_write_png(a_path.c_str(), width, height, channels, data, width * channels);
  }
  else if(a_path.substr(extPos, std::string::npos) == ".jpg" || a_path.substr(extPos, std::string::npos) == ".JPG" ||
          a_path.substr(extPos, std::string::npos) == ".jpeg" || a_path.substr(extPos, std::string::npos) == ".JPEG")
  {
    stbi_write_jpg(a_path.c_str(), width, height, channels, data, 100);
  }
  else
  {
    std::cerr << "Unknown file extension: " << a_path.substr(extPos, std::string::npos) << "in file name" << a_path << "\n";
    return 1;
  }

  return 0;
}

Image::~Image()
{
  if(self_allocated)
    delete [] data;
  else
  {
    stbi_image_free(data);
  }
}

/*
 * drawTile takes x and y in tile coordinates, not in pixels, so it can't be used to draw character sprite
 */
void drawTile(int x, int y, Image &tile, Image &background) {
  for(int y_draw = 0; y_draw < tileSize; ++y_draw)
  {
    for(int x_draw = 0; x_draw < tileSize; ++x_draw)
    {
      background.PutPixel(x * tileSize + x_draw, y * tileSize + y_draw, blend(background.GetPixel(x * tileSize + x_draw, y * tileSize + y_draw), tile.GetPixel(x_draw, tileSize - y_draw - 1)));
    }
  }
}

void initLevel(const std::string &f_path, Image &background, int &starting_x, int &starting_y, std::vector<std::vector<char>> &charMap) {
  Image Wall("../resources/wall.png");
  Image Floor("../resources/floor.png");
  Image Lava("../resources/lava.png");
  Image ChestClosed("../resources/chest.png");
  Image Exit("../resources/exit.png");
  Image Door("../resources/door.png");

  std::ifstream input;
  input.open(f_path);

  int y = 0;
  while (!input.eof()) {
    int x = 0;
    std::string line;
    getline(input, line);
    for (int i = 0; i < line.size(); i++) {
      charMap[x][y] = line[i];
      switch (line[i]) {
        case '#':
          drawTile(x, y, Wall, background);
          break;
        case 'C':
          drawTile(x, y, Floor, background);
          drawTile(x, y, ChestClosed, background);
          break;
        case ' ':
          drawTile(x, y, Lava, background);
          break;
        case '@':
          starting_x = x * tileSize;
          starting_y = y * tileSize;
        case '.':
          drawTile(x, y, Floor, background);
          break;
        case 'x':
          drawTile(x, y, Wall, background);
          drawTile(x, y, Exit, background);
          break;
        case 'D':
          drawTile(x, y, Wall, background);
          drawTile(x, y, Door, background);
          break;
        default:
          break;
      }
      x++;
    }
    y++;
  }
  input.close();
}