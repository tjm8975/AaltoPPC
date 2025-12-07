#include <cmath>
#include <vector>

/*
This is the function you need to implement. Quick reference:
- input rows: 0 <= y < ny
- input columns: 0 <= x < nx
- element at row y and column x is stored in data[x + y*nx]
- correlation between rows i and row j has to be stored in result[i + j*ny]
- only parts with 0 <= j <= i < ny need to be filled

TJM
- Benchmark 1: ~0.47s
- Benchmark 2: ~8.1s (5/5)
*/
void correlate(int ny, int nx, const float *data, float *result)
{
   /* Normalize all rows to have an arithmetic mean of 0 */
   std::vector<double> norm(nx * ny);
   for (int y = 0; y < ny; y++)
   {
      double sum = 0.0;
      for (int x = 0; x < nx; x++)
      {
         sum += data[x + y * nx];
      }

      double avg = sum / nx;
      for (int x = 0; x < nx; x++)
      {
         norm[x + y * nx] = data[x + y * nx] - avg;
      }
   }

   /* Normalize all rows such that the sum of squares is 1 */
   for (int y = 0; y < ny; y++)
   {
      double sum = 0.0;
      for (int x = 0; x < nx; x++)
      {
         sum += norm[x + y * nx] * norm[x + y * nx];
      }

      double normalizer = 1.0 / std::sqrt(sum);
      for (int x = 0; x < nx; x++)
      {
         norm[x + y * nx] = norm[x + y * nx] * normalizer;
      }
   }

   /*
   Multiply normalized matrix by it's transpose.
   Only need to calculate upper right triangle since it will be symmetrical.
   */
   for (int j = 0; j < ny; j++)
   {
      /* Diagonal case */
      result[j + j * ny] = 1;

      for (int i = j + 1; i >= j && i < ny; i++)
      {
         double product = 0.0;

         for (int x = 0; x < nx; x++)
         {
            product += norm[x + i * nx] * norm[x + j * nx];
         }

         result[i + j * ny] = product;
      }
   }
}
