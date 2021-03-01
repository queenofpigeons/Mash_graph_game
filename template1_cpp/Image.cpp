#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>
#include <fstream>
#include <vector>

Pixel blend(Pixel oldPixel, Pixel newPixel)
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

void drawMapFromFile(const std::string &f_path, Image &background) {
  Image Wall("../resources/wall.png");
  Image Floor("../resources/floor.png");
  std::vector<std::vector<char>> charMap(lvlHeight, std::vector<char>(lvlWidth));
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
          for(int y_draw = 0; y_draw < tileSize; ++y_draw)
          {
            for(int x_draw = 0; x_draw < tileSize; ++x_draw)
            {
              background.PutPixel(x * tileSize + x_draw, y * tileSize + y_draw, Wall.GetPixel(x_draw, y_draw));
            }
          }
          break;
        case '.':
          for(int y_draw = 0; y_draw < tileSize; ++y_draw)
          {
            for(int x_draw = 0; x_draw < tileSize; ++x_draw)
            {
              background.PutPixel(x * tileSize + x_draw, y * tileSize + y_draw, Floor.GetPixel(x_draw, y_draw));
            }
          }
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