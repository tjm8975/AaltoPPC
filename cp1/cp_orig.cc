#include <cmath>

/*
This is the function you need to implement. Quick reference:
- input rows: 0 <= y < ny
- input columns: 0 <= x < nx
- element at row y and column x is stored in data[x + y*nx]
- correlation between rows i and row j has to be stored in result[i + j*ny]
- only parts with 0 <= j <= i < ny need to be filled

TJM
- Benchmark 1: ~0.67s
- Benchmark 2: ~11s (3/5)
*/
void correlate(int ny, int nx, const float *data, float *result)
{
   for (int j = 0; j < ny; j++)
   {
      /* Diagonal case */
      result[j + j * ny] = 1;

      double sumY  = 0.0;
      double sumY2 = 0.0;

      for (int x = 0; x < nx; x++)
      {
         double val = data[x + j * nx];
         sumY += val;
         sumY2 += val * val;
      }

      for (int i = j + 1; i >= j && i < ny; i++)
      {
         double sumX  = 0.0;
         double sumX2 = 0.0;
         double sumXY = 0.0;

         for (int x = 0; x < nx; x++)
         {
            double val = data[x + i * nx];
            sumX += val;
            sumX2 += val * val;
            sumXY += val * data[x + j * nx];
         }

         double numerator = (nx * sumXY) - (sumX * sumY);
         double denominator = std::sqrt((nx * sumX2 - (sumX * sumX)) * (nx * sumY2 - (sumY * sumY)));
         result[i + j * ny] = numerator / denominator;
      }
   }
}
