#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>
#include "niblack_algorithm.h"
#include "vec_methods.h"
#include "frame_rectangle.h"
#include "ccl_algorithm.h"

int main()
{
    int max_width = 0, min_width = 0, max_length = 0, min_length = 0, p = 0;
    double max_dens = 0, min_dens = 0, average_dens = 0, average_width = 0, average_length = 0;
    cv::Mat img1 = cv::imread("/home/user/Task_2/test1");
    cv::Mat img = cv::imread("/home/user/Task_2/test1", cv::IMREAD_GRAYSCALE);
    std::vector<std::vector<int>> imgcopy;
    std::vector<std::vector<int>> comps;
    std::vector<int> widths;
    std::vector<int> lengths;
    std::vector<double> dens;
    std::vector<std::vector<uchar>> brightness;

    if (img.empty())
    {
        std::cout << "ERROR" << std::endl;
        return -1;
    }

    for (int i = 0; i < img.rows; i++)
       {
        brightness.push_back(std::vector<uchar>());

        for (int j = 0; j < img.cols; j++)
            {
                brightness[i].push_back(img.at<uchar>(i, j));
            }
       }

    niblack_algorithm(img,brightness);
    ccl_algorithm(img, imgcopy);
    create_set_of_components(imgcopy, comps);


    // 1. IF THE CONNECTIVITY COMPONENT HAS LESS THAN 20 PIXELS, WE DON'T CONSIDER IT
     for (int i = 0; i < comps.size(); i++)
        {
            if (comps[i].size() < 41)
            {
                comps.erase(comps.begin() + i);
                i = i - 1;
            }

        }

        create_frame_parametrs(comps, widths, lengths);
        average_length = average(lengths);
        average_width = average(widths);

    // 2. IF ( WIDTH[I] > 1.5*AVERAGE WIDTH )  OR  IF (HEIGHT[I] > 1.5 * AVERAGE HEIGHT)||(HEIGHT[I] < 0.5 HEIGHT) , WE DON'T CONSIDER IT
     for (int i = 0; (i < widths.size())&&(i<lengths.size()); i++)
        {
            p = 0;
            if (widths[i] > 1.5 * average_width)
            {
                delete_component(comps, widths, lengths, i);
                i = i - 1;
                p = 1;
            }
            if ((p ==0)&&((lengths[i] > 1.2 * average_length)|| (lengths[i] < 0.5 * average_length)))
            {
                delete_component(comps, widths, lengths, i);
                i = i - 1;
            }

        }

     framepaint(img1, comps);

        for (int i = 0; i < comps.size(); i++)
        {
            dens.push_back((double)((comps[i].size() -1)/2)/(double)(widths[i] * lengths[i]));
        }

    average_length = average(lengths);
    average_width = average(widths);
    average_dens = average(dens);

    max_width = maximum(widths);
    min_width = minimum(widths);

    max_length = maximum(lengths);
    min_length = minimum(lengths);

    max_dens = maximum(dens);
    min_dens = minimum(dens);
    //end of calculations

    std::cout<<"max_width = "<<max_width<<std::endl;

    std::cout<<"average_width = "<<average_width<<std::endl;

    std::cout<<"min_width = "<<min_width<<std::endl;

    std::cout<<"max_length = "<<max_length<<std::endl;

    std::cout<<"average_length = "<<average_length<<std::endl;

    std::cout<<"min_length = "<<min_length<<std::endl;

    std::cout<<"max_dens = "<<max_dens<<std::endl;

    std::cout<<"average_dens = "<<average_dens<<std::endl;

    std::cout<<"min_dens = "<<min_dens<<std::endl;

    imshow("Display Window", img1);
    cv::waitKey(0);

   return 0;
}
