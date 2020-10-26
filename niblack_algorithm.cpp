#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>

void makeBlack(cv::Mat img, int i, int j)
{
    if (img.type() != CV_8UC1)
        throw std::runtime_error("Invalid input image");

    img.at<uchar>(i, j) = 0;
}

void makeWhite(cv::Mat img, int i, int j)
{
    if (img.type() != CV_8UC1)
        throw std::runtime_error("Invalid input image");

    img.at<uchar>(i, j) = 255;
}

void niblack_algorithm(cv::Mat img, std::vector<std::vector<uchar>> const& brightness)
{
    double nAverage, average, sqrnDeviation, threshold;
    int Isize = brightness.size();
    int Jsize = brightness[0].size();
    int r = 40; //радиус окрестности, в которой высчитывается формула
    double k = -0.2;

    for (int i = 0; i < Isize; i++)
    {
        int n = (std::min(Isize, i + r + 1) - std::max(0, i - r)) * (r + 1);
        average = nAverage = sqrnDeviation = 0;
        for (int i1 = std::max(0, i - r); i1 < std::min(Isize, i + r + 1); i1++)
            for (int j1 = 0; j1 < r + 1; j1++)
                nAverage += brightness[i1][j1];
        average = nAverage / n;
        for (int i1 = std::max(0, i - r); i1 < std::min(Isize, i + r + 1); i1++)
            for (int j1 = 0; j1 < r + 1; j1++)
            {
                uchar b = brightness[i1][j1];
                sqrnDeviation += (b - average) * (b - average);
            }

        threshold = average + k * sqrt(sqrnDeviation / n);

        if (brightness[i][0] < threshold) makeBlack(img,i, 0); else makeWhite(img, i, 0);

        for (int j = 1; j < Jsize; j++)
        {
            n = (std::min(Jsize, j + r) - std::max(0, j - r)) * (std::min(Isize, i + r) - std::max(0, i - r));
            for (int k1 = i - r; k1 < i + r + 1; k1++)
            {
                uchar b1 = 0;
                if (j - r - 1 >= 0 && k1 >= 0 && k1 < Isize) b1 = brightness[k1][j - r - 1];
                uchar b2 = 0;
                if (j + r < Jsize && k1 >= 0 && k1 < Isize) b2 = brightness[k1][j + r];
                nAverage -= b1;
                nAverage += b2;
            }
            average = nAverage / n;
            for (int k1 = i - r; k1 < i + r + 1; k1++)
            {
                double b1 = average;
                if (j - r - 1 >= 0 && k1 >= 0 && k1 < Isize) b1 = brightness[k1][j - r - 1];
                double b2 = average;
                if (j + r < Jsize && k1 >= 0 && k1 < Isize) b2 = brightness[k1][j + r];
                sqrnDeviation -= (b1 - average) * (b1 - average);
                sqrnDeviation += (b2 - average) * (b2 - average);
            }

            if (sqrnDeviation / n < 256 * 256 / 4) sqrnDeviation *= 2;
            threshold = average + k * sqrt(sqrnDeviation / n);
            if (brightness[i][j] < threshold) makeBlack(img,i, j); else makeWhite(img,i, j);
        }
    }
}
