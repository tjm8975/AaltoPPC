#include <algorithm>
#include <vector>
#include <iostream>

/*
This is the function you need to implement. Quick reference:
- input rows: 0 <= y < ny
- input columns: 0 <= x < nx
- element at row y and column x is stored in in[x + y*nx]
- for each pixel (x, y), store the median of the pixels (a, b) which satisfy
  max(x-hx, 0) <= a < min(x+hx+1, nx), max(y-hy, 0) <= b < min(y+hy+1, ny)
  in out[x + y*nx].

TJM
- Benchmark 1 (100x100, 21x21): 0.058s
- Benchmark 2 (500x500, 21x21): 1.359s
- Benchmark 3 (1000x1000, 21x21): 5.292s
- Benchmark 4 (1500x1500, 21x21): 11.982s (4/5)
*/
void mf(int ny, int nx, int hy, int hx, const float *in, float *out)
{
   std::vector<float> w;

   for (int y = 0; y < ny; y++)
   {
      for (int x = 0; x < nx; x++)
      {
         const int x0 = std::max(x - hx, 0);
         const int x1 = std::min(x + hx + 1, nx);
         const int y0 = std::max(y - hy, 0);
         const int y1 = std::min(y + hy + 1, ny);
         const int wDim = (x1 - x0) * (y1 - y0);
         const int k = wDim / 2;

         for (int wy = y0; wy < y1; wy++)
         {
            for (int wx = x0; wx < x1; wx++)
            {
               w.push_back(in[wx + wy*nx]);
            }
         }

         std::nth_element(w.begin(), w.begin() + k, w.end());

         const float m = w[k];

         if (wDim % 2)
         {
            out[x + y * nx] = m;
         }
         else
         {
            std::nth_element(w.begin(), w.begin() + k - 1, w.end());
            out[x + y * nx] = (m + w[k-1]) * 0.5;
         }

         w.clear();
      }
   }
}
