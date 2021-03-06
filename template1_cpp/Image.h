#ifndef MAIN_IMAGE_H
#define MAIN_IMAGE_H

#include <string>
#include <vector>
#include <GLFW/glfw3.h>

constexpr int tileSize = 32;
constexpr int lvlWidth = 40;
constexpr int lvlHeight = 40;
constexpr int frames = 3;
constexpr int frame_max = 14;
constexpr int frame_divider = 5;

struct Pixel
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

constexpr Pixel backgroundColor{0, 0, 0, 0};
Pixel blend(Pixel oldPixel, Pixel newPixel);

struct Image
{
  explicit Image(const std::string &a_path);
  Image(int a_width, int a_height, int a_channels);
  //Image(int a_width = 32, int a_height = 32, int a_channels = 4);

  int Save(const std::string &a_path);

  int Width()    const { return width; }
  int Height()   const { return height; }
  int Channels() const { return channels; }
  size_t Size()  const { return size; }
  Pixel* Data()        { return  data; }

  Pixel GetPixel(int x, int y) {
    return data[width * y + x];
    }
  void  PutPixel(int x, int y, const Pixel &pix) { data[width * y + x] = pix; }

  ~Image();

private:
  int width = -1;
  int height = -1;
  int channels = 3;
  size_t size = 0;
  Pixel *data = nullptr;
  bool self_allocated = false;
};

void drawTile(int x, int y, Image &tile, Image &background);
void initLevel(const std::string &f_path, Image &background, int &x, int &y, std::vector<std::vector<char>> &charMap);

#endif //MAIN_IMAGE_H
