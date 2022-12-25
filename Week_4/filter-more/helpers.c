#include "math.h"
#include "stdlib.h"
#include "helpers.h"

RGBTRIPLE *getPixelAroundByIndex(int height, int width, RGBTRIPLE image[height][width], int i, int j, int index);
void copyDataFromBufferToImage(int height, int width, RGBTRIPLE buffer[height][width], RGBTRIPLE image[height][width]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE *pixel = &image[i][j];

            BYTE average_color = round((pixel->rgbtRed + pixel->rgbtGreen + pixel->rgbtBlue) / 3.0);

            pixel->rgbtRed = average_color;
            pixel->rgbtGreen = average_color;
            pixel->rgbtBlue = average_color;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int start = 0; start < (width / 2); start++)
        {
            int end = width - 1 - start;

            RGBTRIPLE temp = image[i][start];
            image[i][start] = image[i][end];
            image[i][end] = temp;
        }
    }
    return;
}

// Return ptr to the pixel around current one by i,j according to the 3x3 grid or NULL
RGBTRIPLE *getPixelAroundByIndex(int height, int width, RGBTRIPLE image[height][width], int i, int j, int index)
{
    switch (index)
    {
        case 0:
            if (i - 1 < 0 || j - 1 < 0)
            {
                return NULL;
            }
            return &image[i - 1][j - 1];
        case 1:
            if (i - 1 < 0)
            {
                return NULL;
            }
            return &image[i - 1][j];
        case 2:
            if (i - 1 < 0 || j + 1 == width)
            {
                return NULL;
            }
            return &image[i - 1][j + 1];
        case 3:
            if (j - 1 < 0)
            {
                return NULL;
            }
            return &image[i][j - 1];
        case 4:
            return &image[i][j];
        case 5:
            if (j + 1 == width)
            {
                return NULL;
            }
            return &image[i][j + 1];
        case 6:
            if (i + 1 == height || j - 1 < 0)
            {
                return NULL;
            }
            return &image[i + 1][j - 1];
        case 7:
            if (i + 1 == height)
            {
                return NULL;
            }
            return &image[i + 1][j];
        case 8:
            if (i + 1 == height || j + 1 == width)
            {
                return NULL;
            }
            return &image[i + 1][j + 1];
        default:
            return NULL;
    }
}

void copyDataFromBufferToImage(int height, int width, RGBTRIPLE buffer[height][width], RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = buffer[i][j];
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE buffer[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float amount = 0;
            int sumRed = 0;
            int sumGreen = 0;
            int sumBlue = 0;

            // Get pixels around the current pixel one by one
            for (int k = 0; k < 9; k++)
            {
                RGBTRIPLE *pixel = getPixelAroundByIndex(height, width, image, i, j, k);

                if (!pixel)
                {
                    continue;
                }

                amount++;
                sumRed += pixel->rgbtRed;
                sumGreen += pixel->rgbtGreen;
                sumBlue += pixel->rgbtBlue;
            }

            buffer[i][j].rgbtRed = round(sumRed / amount);
            buffer[i][j].rgbtGreen = round(sumGreen / amount);
            buffer[i][j].rgbtBlue = round(sumBlue / amount);
        }
    }
    copyDataFromBufferToImage(height, width, buffer, image);
    return;
}

// Constant kernels for getting the edges on the image, aligned according to the 3x3 grid
const int Gx[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1}; // x axis
const int Gy[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1}; // y axis

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE buffer[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int xAxisSum[] = {0, 0, 0}; // sum of red, green, blue in x axis
            int yAxisSum[] = {0, 0, 0}; // sum of red, green, blue in y axis

            // Get pixels around the current pixel one by one
            for (int k = 0; k < 9; k++)
            {
                RGBTRIPLE *pixel = getPixelAroundByIndex(height, width, image, i, j, k);

                if (!pixel)
                {
                    continue;
                }

                xAxisSum[0] += pixel->rgbtRed * Gx[k];
                xAxisSum[1] += pixel->rgbtGreen * Gx[k];
                xAxisSum[2] += pixel->rgbtBlue * Gx[k];

                yAxisSum[0] += pixel->rgbtRed * Gy[k];
                yAxisSum[1] += pixel->rgbtGreen * Gy[k];
                yAxisSum[2] += pixel->rgbtBlue * Gy[k];
            }

            int rgbtRed = round(sqrt(pow(xAxisSum[0], 2) + pow(yAxisSum[0], 2)));
            int rgbtGreen = round(sqrt(pow(xAxisSum[1], 2) + pow(yAxisSum[1], 2)));
            int rgbtBlue = round(sqrt(pow(xAxisSum[2], 2) + pow(yAxisSum[2], 2)));

            buffer[i][j].rgbtRed = rgbtRed > 255 ? 255 : rgbtRed;
            buffer[i][j].rgbtGreen = rgbtGreen > 255 ? 255 : rgbtGreen;
            buffer[i][j].rgbtBlue = rgbtBlue > 255 ? 255 : rgbtBlue;
        }
    }
    copyDataFromBufferToImage(height, width, buffer, image);
    return;
}
