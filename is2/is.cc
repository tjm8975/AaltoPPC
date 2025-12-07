#include <vector>
#include <algorithm>

struct Result {
    int y0;
    int x0;
    int y1;
    int x1;
    float outer[3];
    float inner[3];
};



/*
This is the function you need to implement. Quick reference:
- x coordinates: 0 <= x < nx
- y coordinates: 0 <= y < ny
- color components: 0 <= c < 3
- input: data[c + 3 * x + 3 * nx * y]

TJM
- Benchmark 1  (30x30): 0.065s
- Benchmark 2a (39x39): 0.154s
- Benchmark 2b (40x40): 0.161s
- Benchmark 2c (41x41): 0.159s
- Benchmark 3  (50x50): 0.267s (4/5)
*/
Result segment(int ny, int nx, const float *data) {
    Result result{0, 0, 0, 0, {0, 0, 0}, {0, 0, 0}};

    std::vector<double> sumByRec(nx * ny * 3);
    std::vector<double> rowSum(3);
    for (int x = 0; x < nx; x++)
    {
        for (int c = 0; c < 3; c++)
        {
            rowSum[c] += data[3 * x + c];
            sumByRec[3 * x + c] = rowSum[c];
        }
    }
    
    for (int y = 1; y < ny; y++)
    {
        rowSum = {0.0, 0.0, 0.0};
        for (int x = 0; x < nx; x++)
        {
            for (int c = 0; c < 3; c++)
            {
                rowSum[c] += data[3 * x + 3 * nx * y + c];
                sumByRec[3 * x + 3 * nx * y + c] = sumByRec[3 * x + 3 * nx * (y - 1) + c] + rowSum[c];
            }
        }
    }

    std::vector<double> t1(&sumByRec[nx * ny * 3 - 3], &sumByRec[nx * ny * 3]);

    double bestScore = 0.0;

    /* Loop over all possible rectangle dimensions */
    for (int y = 1; y <= ny; y++)
    {
        for (int x = 1; x <= nx; x++)
        {
            const double innerFracN = 1.0 / (x * y);
            const int numOuterPixels = (nx * ny) - (x * y);
            const double outerFracN = numOuterPixels == 0.0 ? 0 : 1.0 / numOuterPixels;

            /* Loop over all possible positions for the current rectangle dimensions */
            for (int y0 = 0; y0 <= ny - y; y0++)
            {
                int y1 = y0 + y;
                for (int x0 = 0; x0 <= nx - x; x0++)
                {
                    int x1 = x0 + x;

                    /* Handle case where x0 and/or y0 are 0 to avoid inavlid memory accesses */
                    int x0Idx = x0 == 0 ? 0 : x0 - 1;
                    int y0Idx = y0 == 0 ? 0 : y0 - 1;

                    /* Get sum of color values for rectangle spanning from (0,0) to (x0,y0) */
                    std::vector<double> cornerRec(&sumByRec[3 * (x0Idx) + 3 * nx * (y0Idx)],
                                                  &sumByRec[3 * (x0Idx) + 3 * nx * (y0Idx) + 3]);

                    /* Get sum of color values for rectangle spanning from (0,0) to (x1,y0) */
                    std::vector<double> upperRec(&sumByRec[3 * (x1 - 1) + 3 * nx * (y0Idx)],
                                                 &sumByRec[3 * (x1 - 1) + 3 * nx * (y0Idx) + 3]);

                    /* Get sum of color values for rectangle spanning from (0,0) to (x0,y1) */
                    std::vector<double> leftRec(&sumByRec[3 * (x0Idx) + 3 * nx * (y1 - 1)],
                                                &sumByRec[3 * (x0Idx) + 3 * nx * (y1 - 1) + 3]);

                    /* Get sum of color values for rectangle spanning from (0,0) to (x1,y1) */
                    std::vector<double> bigRec(&sumByRec[3 * (x1 - 1) + 3 * nx * (y1 - 1)],
                                               &sumByRec[3 * (x1 - 1) + 3 * nx * (y1 - 1) + 3]);

                    /* Calculate sum of inner rectangle
                       There is probably a better way to do this, but it works for the baseline solution */
                    std::vector<double> innerSum(3);
                    if (x0 == 0 && y0 == 0)
                    {
                        innerSum = bigRec;
                    }
                    else if (x0 == 0)
                    {
                        for (int c = 0; c < 3; c++)
                        {
                            innerSum[c] = bigRec[c] - upperRec[c];
                        }
                    }
                    else if (y0 == 0)
                    {
                        for (int c = 0; c < 3; c++)
                        {
                            innerSum[c] = bigRec[c] - leftRec[c];
                        }
                    }
                    else
                    {
                        for (int c = 0; c < 3; c++)
                        {
                            innerSum[c] = bigRec[c] - upperRec[c] - leftRec[c] + cornerRec[c];
                        }
                    }

                    std::vector<double> outerSum(3);
                    std::vector<double> innerColor(3);
                    std::vector<double> outerColor(3);
                    double posScore = 0.0;
                    for (int c = 0; c < 3; c++)
                    {
                        outerSum[c] = t1[c] - innerSum[c];
                        innerColor[c] = innerSum[c] * innerFracN;
                        outerColor[c] = outerSum[c] * outerFracN;
                        posScore += innerSum[c] * innerColor[c] + outerSum[c] * outerColor[c];
                    }

                    if (posScore > bestScore)
                    {
                        bestScore = posScore;
                        result.y0 = y0;
                        result.x0 = x0;
                        result.y1 = y1;
                        result.x1 = x1;
                        for (int c = 0; c < 3; c++)
                        {
                            result.outer[c] = outerColor[c];
                            result.inner[c] = innerColor[c];
                        }
                    }
                }
            }
        }
    }

    return result;
}