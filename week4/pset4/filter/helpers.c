#include "helpers.h"
#include <math.h>
#include <stdlib.h>

// Convert image to grayscale
// Avg of each px's RGB
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // Get og value
            float blue = image[h][w].rgbtBlue;
            float green = image[h][w].rgbtGreen;
            float red = image[h][w].rgbtRed;

            // Calculate average
            int avg = round((red + blue + green) / 3);

            // Set all values to average
            image[h][w].rgbtBlue = image[h][w].rgbtGreen = image[h][w].rgbtRed = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Only need to go through the left half
    for (int w = 0; w < width / 2; w++)
    {
        for (int h = 0; h < height; h++)
        {
            RGBTRIPLE opp = image[h][width - w - 1];
            image[h][width - w - 1] = image[h][w];
            image[h][w] = opp;
        }
    }
    return;
}

// Blur image
// Box blur: avg of the 3x3 box, less for the edges
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE UL, L, LL, UC, LC, UR, R, LR;

    int number;
    RGBTRIPLE *pix = (RGBTRIPLE *) malloc(width * height * sizeof(RGBTRIPLE));
    RGBTRIPLE tmp;
    float red, green, blue;

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {

            // Center
            if (h > 0 && (h < (height - 1)) && (w > 0) && (w < (width - 1)))
            {
                number = 9;
                UL = image[h - 1][w - 1];
                L = image[h][w - 1];
                LL = image[h + 1][w - 1];
                UC = image[h - 1][w];
                LC = image[h + 1][w];
                UR = image[h - 1][w + 1];
                R = image[h][w + 1];
                LR = image[h + 1][w + 1];
            }

            // Upper Edge - UL, UC, UR
            if ((h == 0) && (w > 0) && w < (width - 1))
            {
                number = 6;
                UL.rgbtBlue = 0;
                UL.rgbtGreen = 0;
                UL.rgbtRed = 0;
                UC.rgbtBlue = 0;
                UC.rgbtGreen = 0;
                UC.rgbtRed = 0;
                UR.rgbtBlue = 0;
                UR.rgbtGreen = 0;
                UR.rgbtRed = 0;

                // Existent values
                L = image[h][w - 1];
                LL = image[h + 1][w - 1];
                LC = image[h + 1][w];
                R = image[h][w + 1];
                LR = image[h + 1][w + 1];
            }

            // Lower Edge - LL, LC, LR
            if ((h == (height - 1)) && (w > 0) && w < (width - 1))
            {
                number = 6;
                LL.rgbtBlue = 0;
                LL.rgbtGreen = 0;
                LL.rgbtRed = 0;
                LC.rgbtBlue = 0;
                LC.rgbtGreen = 0;
                LC.rgbtRed = 0;
                LR.rgbtBlue = 0;
                LR.rgbtGreen = 0;
                LR.rgbtRed = 0;

                // Existent values
                UL = image[h - 1][w - 1];
                L = image[h][w - 1];
                UC = image[h - 1][w];
                UR = image[h - 1][w + 1];
                R = image[h][w + 1];
            }

            // Left Edge - UL, L, LL
            if (w == 0 && h > 0 && h < (height - 1))
            {
                number = 6;
                UL.rgbtBlue = 0;
                UL.rgbtGreen = 0;
                UL.rgbtRed = 0;
                L.rgbtBlue = 0;
                L.rgbtGreen = 0;
                L.rgbtRed = 0;
                LL.rgbtBlue = 0;
                LL.rgbtGreen = 0;
                LL.rgbtRed = 0;

                // Existent values
                UC = image[h - 1][w];
                LC = image[h + 1][w];
                UR = image[h - 1][w + 1];
                R = image[h][w + 1];
                LR = image[h + 1][w + 1];
            }

            // Right Edge - UR, R, LR
            if ((w == (width - 1)) && (h > 0) && (h < (height - 1)))
            {
                number = 6;
                UR.rgbtBlue = 0;
                UR.rgbtGreen = 0;
                UR.rgbtRed = 0;
                R.rgbtBlue = 0;
                R.rgbtGreen = 0;
                R.rgbtRed = 0;
                LR.rgbtBlue = 0;
                LR.rgbtGreen = 0;
                LR.rgbtRed = 0;

                // Existent values
                UL = image[h - 1][w - 1];
                L = image[h][w - 1];
                LL = image[h + 1][w - 1];
                UC = image[h - 1][w];
                LC = image[h + 1][w];
            }

            // UL corner - UR, UC, UL, L, LL
            if ((w == 0) && (h == 0))
            {
                number = 4;
                UR.rgbtBlue = 0;
                UR.rgbtGreen = 0;
                UR.rgbtRed = 0;
                UC.rgbtBlue = 0;
                UC.rgbtGreen = 0;
                UC.rgbtRed = 0;
                UL.rgbtBlue = 0;
                UL.rgbtGreen = 0;
                UL.rgbtRed = 0;
                L.rgbtBlue = 0;
                L.rgbtGreen = 0;
                L.rgbtRed = 0;
                LL.rgbtBlue = 0;
                LL.rgbtGreen = 0;
                LL.rgbtRed = 0;

                // Existent values
                LC = image[1][0];
                R = image[0][1];
                LR = image[1][1];
            }

            // UR corner - UL, UC, UR, R, LR
            if ((w == (width - 1)) && (h == 0))
            {
                number = 4;
                UL.rgbtBlue = 0;
                UL.rgbtGreen = 0;
                UL.rgbtRed = 0;
                UC.rgbtBlue = 0;
                UC.rgbtGreen = 0;
                UC.rgbtRed = 0;
                UR.rgbtBlue = 0;
                UR.rgbtGreen = 0;
                UR.rgbtRed = 0;
                R.rgbtBlue = 0;
                R.rgbtGreen = 0;
                R.rgbtRed = 0;
                LR.rgbtBlue = 0;
                LR.rgbtGreen = 0;
                LR.rgbtRed = 0;

                // Existent values
                L = image[0][w - 1];
                LL = image[1][w - 1];
                LC = image[1][w];
            }

            // LL corner - UL, L, LL, LC, LR
            if ((w == 0) && (h == (height - 1)))
            {
                number = 4;
                UL.rgbtBlue = 0;
                UL.rgbtGreen = 0;
                UL.rgbtRed = 0;
                L.rgbtBlue = 0;
                L.rgbtGreen = 0;
                L.rgbtRed = 0;
                LL.rgbtBlue = 0;
                LL.rgbtGreen = 0;
                LL.rgbtRed = 0;
                LC.rgbtBlue = 0;
                LC.rgbtGreen = 0;
                LC.rgbtRed = 0;
                LR.rgbtBlue = 0;
                LR.rgbtGreen = 0;
                LR.rgbtRed = 0;

                // Existent values
                UC = image[h - 1][0];
                UR = image[h - 1][1];
                R = image[h][1];
            }

            //LR corner - UR, R, LR, LC, LL
            if ((w == (width - 1)) && (h == (height - 1)))
            {
                number = 4;
                UR.rgbtBlue = 0;
                UR.rgbtGreen = 0;
                UR.rgbtRed = 0;
                R.rgbtBlue = 0;
                R.rgbtGreen = 0;
                R.rgbtRed = 0;
                LR.rgbtBlue = 0;
                LR.rgbtGreen = 0;
                LR.rgbtRed = 0;
                LC.rgbtBlue = 0;
                LC.rgbtGreen = 0;
                LC.rgbtRed = 0;
                LL.rgbtBlue = 0;
                LL.rgbtGreen = 0;
                LL.rgbtRed = 0;

                // Existent values
                UL = image[h - 1][w - 1];
                L = image[h][w - 1];
                UC = image[h - 1][w];
            }

            // Calculate Sum
            blue = UL.rgbtBlue + L.rgbtBlue + LL.rgbtBlue + UC.rgbtBlue + image[h][w].rgbtBlue + LC.rgbtBlue + UR.rgbtBlue + R.rgbtBlue +
                   LR.rgbtBlue;
            green = UL.rgbtGreen + L.rgbtGreen + LL.rgbtGreen + UC.rgbtGreen + image[h][w].rgbtGreen + LC.rgbtGreen + UR.rgbtGreen + R.rgbtGreen
                    + LR.rgbtGreen;
            red = UL.rgbtRed + L.rgbtRed + LL.rgbtRed + UC.rgbtRed + image[h][w].rgbtRed + LC.rgbtRed + UR.rgbtRed + R.rgbtRed + LR.rgbtRed;

            // Store in temporary variable
            tmp.rgbtBlue = (int) round(blue / number);
            tmp.rgbtGreen = (int) round(green / number);
            tmp.rgbtRed = (int) round(red / number);
            *(pix + h * width + w) = tmp;
        }
    }

    // Retrieve info from malloc-ed space
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = *(pix + h * width + w);
        }
    }

    free(pix);
    return;
}

// Detect edges
/* Sobel operator:
 * Weighted sum of each of the R,G,B of the surrounding 3x3 box
 * One sum of Gx and Gy each
 * sqrt(Gx^2 + Gy^2) (rounded, capped at 255)
 */
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Tmp array
    RGBTRIPLE tmp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tmp[i][j] = image[i][j];
        }
    }

    // Sobel arrays
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // Initialize variables
            float GxR;
            float GxB;
            float GxG;
            float GyR;
            float GyB;
            float GyG;

            GxR = GxB = GxG = GyR = GyB = GyG = 0;

            // For each pixel, loop vertical and horizontal
            for (int i = -1; i < 2; i++)
            {
                for (int j = -1; j < 2; j++)
                {
                    // Check if pixel is outside rows
                    if (h + i < 0 || h + i >= height)
                    {
                        continue;
                    }

                    // Check if pixel is outside columns
                    if (w + j < 0 || w + j >= width)
                    {
                        continue;
                    }

                    // Otherwise add to sums
                    GxR += tmp[h + i][w + j].rgbtRed * Gx[i + 1][j + 1];
                    GxG += tmp[h + i][w + j].rgbtGreen * Gx[i + 1][j + 1];
                    GxB += tmp[h + i][w + j].rgbtBlue * Gx[i + 1][j + 1];
                    GyR += tmp[h + i][w + j].rgbtRed * Gy[i + 1][j + 1];
                    GyG += tmp[h + i][w + j].rgbtGreen * Gy[i + 1][j + 1];
                    GyB += tmp[h + i][w + j].rgbtBlue * Gy[i + 1][j + 1];
                }
            }

            // Calculate Sobel
            int red = round(sqrt(GxR * GxR + GyR * GyR));
            int green = round(sqrt(GxG * GxG + GyG * GyG));
            int blue = round(sqrt(GxB * GxB + GyB * GyB));

            // Check upper bound
            if (blue > 255)
            {
                blue = 255;
            }
            if (green > 255)
            {
                green = 255;
            }
            if (red > 255)
            {
                red = 255;
            }

            // Assign new values to pixels
            image[h][w].rgbtRed = red;
            image[h][w].rgbtGreen = green;
            image[h][w].rgbtBlue = blue;
        }
    }
    return;
}
