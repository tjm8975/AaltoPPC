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
- Benchmark 1: 0.172s
- Benchmark 2: 3.66s (3/3)
*/
void correlate(int ny, int nx, const float *data, float *result)
{
   constexpr int na = 4;
   int nb = nx / na;
   int nab = na * nb;

   /* TODO: Try with vector instead */
   double sumVec[na];
   for (int i = 0; i < na; i++)
   {
      sumVec[i] = 0.0;
   }

   /* Normalize all rows to have an arithmetic mean of 0 */
   std::vector<double> norm(nx * ny);
   for (int y = 0; y < ny; y++)
   {
      // TODO: Try having 2 accumulators for each of the sums in the various loops
      // They all loop over x, so can just have x/2 iterations and second loop that covers x%2 values
      // Once that is working, try again with 4 accumulators
      for (int x = 0; x < nb; x++)
      {
         for (int i = 0; i < na; i++)
         {
            sumVec[i] += data[x * na + y * nx + i];
         }
      }

      /* Handle leftover elements of row */
      for (int i = 0; i < nx - nab; i++)
      {
         sumVec[i] += data[nab + y * nx + i];
      }

      /* Combine for total sum of row */
      double sum = 0.0;
      for (int i = 0; i < na; i++)
      {
         sum += sumVec[i];
         sumVec[i] = 0.0;
      }

      double avg = sum / nx;
      for (int x = 0; x < nb; x++)
      {
         for (int i = 0; i < na; i++)
         {
            norm[x * na + y * nx + i] = data[x * na + y * nx + i] - avg;
            sumVec[i] += norm[x * na + y * nx + i] * norm[x * na + y * nx + i];
         }
      }

      /* Handle leftover elements of row */
      for (int i = 0; i < nx - nab; i++)
      {
         norm[nab + y * nx + i] = data[nab + y * nx + i] - avg;
         sumVec[i] += norm[nab + y * nx + i] * norm[nab + y * nx + i];
      }

      /* Combine for total sum of squares */
      sum = 0.0;
      for (int i = 0; i < na; i++)
      {
         sum += sumVec[i];
         sumVec[i] = 0.0;
      }

      /* Normalize all rows such that the sum of squares is 1 */
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
         for (int x = 0; x < nb; x++)
         {
            for (int k = 0; k < na; k++)
            {
               sumVec[k] += norm[x * na + j * nx + k] * norm[x * na + i * nx + k];
            }
         }

         /* Handle leftover elements of row */
         for (int k = 0; k < nx - nab; k++)
         {
            sumVec[k] += norm[nab + j * nx + k] * norm[nab + i * nx + k];
         }

         /* Combine for total sum of squares */
         double sum = 0.0;
         for (int k = 0; k < na; k++)
         {
            sum += sumVec[k];
            sumVec[k] = 0.0;
         }

         result[i + j * ny] = sum;
      }
   }
}
