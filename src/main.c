#include <png.h>
#include <stdlib.h>

#define WIDTH 500
#define HEIGHT 500
#define CX -0.4
#define CY 0.6
#define X_MIN -1.5
#define X_MAX 1.5
#define Y_MIN -1.5
#define Y_MAX 1.5
#define MAX_ITERATIONS 1000
#define MAX_COLOR_VALUE 255
#define OUTPUT_FILE "output.png"

int main() {

  // ジュリア集合を描写する
  // 画像の幅と高さを指定する
  int width = WIDTH;
  int height = HEIGHT;

  // 画像のデータを格納する配列を確保する
  png_bytep *row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
  for (int y = 0; y < height; y++) {
    row_pointers[y] = (png_byte*)malloc(sizeof(png_byte) * width * 4);
  }

  // ジュリア集合を描写する
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {

      // 座標を計算する
      double zx = X_MIN + (X_MAX - X_MIN) * x / width;
      double zy = Y_MIN + (Y_MAX - Y_MIN) * y / height;

      // ジュリア集合を描写する
      int i = 0;
      while (zx * zx + zy * zy < 4 && i < MAX_ITERATIONS) {
        double tmp = zx * zx - zy * zy + CX;
        zy = 2 * zx * zy + CY;
        zx = tmp;
        i++;
      }

      // 色を計算する
      int r = i % MAX_COLOR_VALUE;
      int g = i % MAX_COLOR_VALUE;
      int b = i % MAX_COLOR_VALUE;
      int a = MAX_COLOR_VALUE;

      // 色を画像に書き込む
      row_pointers[y][x * 4 + 0] = r;
      row_pointers[y][x * 4 + 1] = g;
      row_pointers[y][x * 4 + 2] = b;
      row_pointers[y][x * 4 + 3] = a;

    }
  }

  // 画像をファイルに出力する
  FILE *fp = fopen(OUTPUT_FILE, "wb");
  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  png_infop info = png_create_info_struct(png);
  png_init_io(png, fp);
  png_set_IHDR(
    png,
    info,
    width, height,
    8,
    PNG_COLOR_TYPE_RGBA,
    PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT
  );

  png_write_info(png, info);
  png_write_image(png, row_pointers);
  png_write_end(png, NULL);

  // メモリを解放する
  for (int y = 0; y < height; y++) {
    free(row_pointers[y]);
  }
  free(row_pointers);

  return 0;

}
