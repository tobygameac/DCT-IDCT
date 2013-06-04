#include <stdio.h>
#include <math.h>
#define N 512

double COS[8][8], C[8];
unsigned char pic[N][N];
double dct[N][N], idct[N][N];

void init() {
  int i, j;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++)
      COS[i][j] = cos((2 * i + 1) * j * acos(-1) / 16.0);
    if (i) C[i] = 1;
    else C[i] = 1 / sqrt(2);
  }
}

void DCT() {
  freopen("dct.raw", "wb", stdout);
  int r, c, i, j, x, y;
  for (r = 0; r < 64; r++)
    for (c = 0; c < 64; c++)
      for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++) {
          double sum = 0;
          for (x = 0; x < 8; x++)
            for (y = 0; y < 8; y++)
              sum += (pic[r * 8 + x][c * 8 + y] - 128) * COS[x][i] * COS[y][j];
          sum *= C[i] * C[j] * 0.25;
          dct[r * 8 + i][c * 8 + j] = sum;
      }
  for (r = 0; r < N; r++)
    for (c = 0; c < N; c++)
      putchar(dct[r][c]);
}

void IDCT() {
  freopen("idct.raw", "wb", stdout);
  int r, c, i, j, x, y;
  for (r = 0; r < 64; r++)
    for (c = 0; c < 64; c++)
      for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++) {
          double sum = 0;
          for (x = 0; x < 8; x++)
            for (y = 0; y < 8; y++)
              sum += C[x] * C[y] * dct[r * 8 + x][c * 8 + y] * COS[i][x] * COS[j][y];
          sum *= 0.25;
          sum += 128;
          idct[r * 8 + i][c * 8 + j] = sum;
      }
  for (r = 0; r < N; r++)
    for (c = 0; c < N; c++)
      putchar(idct[r][c]);
}

void quantization() {
  int table[8][8] = {
    16, 11, 10, 16, 24, 40, 51, 61,
    12, 12, 14, 19, 26, 58, 60, 55,
    14, 13, 16, 24, 40, 57, 69, 56,
    14, 17, 22, 29, 51, 87, 80, 82,
    18, 22, 37, 56, 68, 109, 103, 77,
    24, 35, 55, 64, 81, 104, 113, 92,
    49, 64, 78, 87, 103, 121, 120, 101,
    72, 92, 95, 98, 112, 100, 103, 99
  };
  int r, c, i, j;
  for (r = 0; r < 64; r++)
    for (c = 0; c < 64; c++)
      for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++) {
          dct[r * 8 + i][c * 8 + j] = round(dct[r * 8 + i][c * 8 + j] / table[i][j]);
          dct[r * 8 + i][c * 8 + j] = dct[r * 8 + i][c * 8 + j] * table[i][j];
        }
}

void MSE() {
  freopen("sub.raw", "wb", stdout);
  double MSE = 0;
  int r, c;
  for (r = 0; r < N; r++)
    for (c = 0; c < N; c++) {
      MSE += (pic[r][c] - idct[r][c]) * (pic[r][c] - idct[r][c]);
      putchar(abs(pic[r][c] - idct[r][c]));
    }
  MSE /= (512 * 512);
  double PSNR = 10 * log10(255 * 255 / MSE);
  freopen("PSNR.txt", "w", stdout);
  printf("%.2lf\n", PSNR);
}

int main() {
  freopen("Lena.raw", "rb", stdin);
  int r, c;
  for (r = 0; r < N; r++)
    for (c = 0; c < N; c++)
      scanf("%c", &pic[r][c]);
  init();
  DCT();
  quantization();
  IDCT();
  MSE();
  return 0;
}
