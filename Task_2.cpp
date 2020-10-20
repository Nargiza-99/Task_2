#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
//#include <fstream>
#include <vector>

bool isBlack(cv::Mat img, int i, int j)
{
    if(img.type() != CV_8UC3)
        throw std::runtime_error("Invalid input image");
    if (img.at<cv::Vec3b>(i, j)[0] + img.at<cv::Vec3b>(i, j)[1] +img.at<cv::Vec3b>(i, j)[2] < 500)
        return 1;
    else
        return 0;
}

void makeBlack(cv::Mat img, int i, int j)
{
    if(img.type() != CV_8UC3)
        throw std::runtime_error("Invalid input image");
for (int k = 0; k <  3; k++)
    img.at<cv::Vec3b>(i, j)[k] = 0;
}

void makeBlue(cv::Mat img, int i, int j)
{
    if(img.type() != CV_8UC3)
        throw std::runtime_error("Invalid input image");
    for (int k = 1; k < 3; k++)
        img.at<cv::Vec3b>(i, j)[k] = 0;
    img.at<cv::Vec3b>(i, j)[0] = 255;
}

void makeYellow(cv::Mat img, int i, int j)
{
    if(img.type() != CV_8UC3)
        throw std::runtime_error("Invalid input image");
    for (int k = 1; k < 3; k++)
        img.at<cv::Vec3b>(i, j)[k] = 255;
    img.at<cv::Vec3b>(i, j)[0] = 0;
}

void makeWhite(cv::Mat img, int i, int j)
{
    if(img.type() != CV_8UC3)
        throw std::runtime_error("Invalid input image");
    for (int k = 0; k <  3; k++)
        img.at<cv::Vec3b>(i, j)[k] = 255;
}

int find_(int X, std::vector<int> const& linked)
{
    int j = X;
    while (linked[j-1] != 0)
        j = linked[j-1];
    return j;

}

void union_(int X, int Y, std::vector<int>& linked)
{
    int j = find_(X, linked);
    int k = find_(Y, linked);
    if (j != k)
        linked[k-1] = j;
}

double vecMed(std::vector<double>& vec)
{
    if(vec.empty()) return 0;
    else
    {
    std::vector<int> copy;
            for (int i=0; i< vec.size(); i++)
                 copy.push_back(vec[i]);
    std::sort(copy.begin(), copy.end());
    if(vec.size() % 2 == 0)
    return (vec[vec.size()/2 - 1] + vec[vec.size()/2]) / 2;
    else
    return vec[vec.size()/2];
    }
}
int vecMed(std::vector<int>& vec)
{
    if(vec.empty()) return 0;
    else
    {
    std::vector<int> copy;
            for (int i=0; i< vec.size(); i++)
                 copy.push_back(vec[i]);
    std::sort(copy.begin(), copy.end());
    if(vec.size() % 2 == 0)
    return (vec[vec.size()/2 - 1] + vec[vec.size()/2]) / 2;
    else
    return vec[vec.size()/2];
    }
}
int main()
{
    int label = 0, B = 0, C = 0, p = 0, max_j = 0, min_j = 0, max_i = 0, min_i = 0, index = 0, average_width = 0, average_length = 0, help1 = 0, help2 = 0, help3 = 0;
    int max_width = 0, min_width = 0, max_length = 0, min_length = 0, max_dens = 0, min_dens = 0, average_dens = 0;
    double average_ratio = 0;
    cv::Mat img = cv::imread("/home/user/Task_2/test1");
    std::vector<std::vector<int>> imgcopy;
    std::vector<int> linked;
    std::vector<std::vector<int>> comps;
    std::vector<int> use;
    std::vector<int> widths;
    std::vector<int> lengths;
    std::vector<int> dens;
    std::vector<double> ratio;


    if (img.empty())
    {
        std::cout << "ERROR" << std::endl;
        return -1;
    }
    //connected-component label algorithm
        for (int i = 0; i < img.rows; i++)
        {
            imgcopy.push_back(std::vector<int>());

            for (int j = 0; j < img.cols; j++)
                {
                if (!isBlack(img,i,j))
                {
                    imgcopy[i].push_back(0);
                }
                else
                {

                    if (i == 0)
                    {
                        if (j == 0)
                        {
                            label = label + 1;
                            linked.push_back(0);
                            imgcopy[i].push_back(label);
                        }
                        if (j > 0)
                        {
                            if (imgcopy[i][j - 1] != 0)
                            {
                                imgcopy[i].push_back(imgcopy[i][j - 1]);
                            }
                            else
                            {
                                label = label + 1;
                                linked.push_back(0);
                                imgcopy[i].push_back(label);
                            }

                        }
                    }
                    if ((j == 0)&&(i!=0))
                    {
                        if (imgcopy[i - 1][j] != 0)
                        {
                            imgcopy[i].push_back(imgcopy[i - 1][j]);
                        }
                        else
                        {
                            label = label + 1;
                            linked.push_back(0);
                            imgcopy[i].push_back(label);
                         }



                    }
                    if ((i > 0) && (j > 0))
                    {
                        B = imgcopy[i][j - 1];
                        C = imgcopy[i - 1][j];
                        if ((B == 0) && (C == 0))
                        {
                            label = label + 1;
                            linked.push_back(0);
                            imgcopy[i].push_back(label);

                        }
                        if ((B != 0) && (C == 0))
                        {
                            imgcopy[i].push_back(B);
                        }

                        if ((B == 0) && (C != 0))
                        {
                            imgcopy[i].push_back(C);
                        }
                        if ((B != 0) && (C != 0))
                        {
                            if (B == C)
                            {
                                imgcopy[i].push_back(B);

                            }
                            else
                            {
                                if (B > C)
                                {
                                    linked[B-1] = C;
                                    imgcopy[i].push_back(C);
                                    union_(C, B, linked);

                                }
                                else
                                {
                                    linked[C-1] = B;
                                    imgcopy[i].push_back(B);
                                    union_(B, C, linked);

                                }

                            }
                        }
                    }
                }
                }
        }
        for (int i = 0; i < imgcopy.size(); i++)
        {
            for (int j = 0; j < imgcopy[i].size(); j++)
            {
                if (isBlack(img,i,j))
                    imgcopy[i][j] = find_(imgcopy[i][j], linked);
            }
        }
    //end of connected-component label algorithm

    //creating of vectors, containing connected components  :  v1 = (label, i1,j1, i2,j2, i3,j3, ... ); where (i,j) - coordinates of point, contained in the connectivity component;
    // comps = (v1, v2, ... vn) , where n - number of connectivity components
        for (int i = 0; i < imgcopy.size(); i++)
        {
            for (int j = 0; j < imgcopy[i].size(); j++)
            {
                p = 0;
                if (imgcopy[i][j] != 0)
                {
                    for (int m = 0; m < comps.size(); m++)
                        if (comps[m][0] == imgcopy[i][j])
                        {
                            comps[m].push_back(i);
                            comps[m].push_back(j);
                            p = 1;
                        }
                    if (p == 0)
                    {
                        use.push_back(imgcopy[i][j]);
                        comps.push_back(use);
                        use.pop_back();
                    }
                }

            }
        }

    // 1. IF THE CONNECTIVITY COMPONENT HAS LESS THAN 20 PIXELS, WE DON'T CONSIDER IT
        for (int i = 0; i < comps.size(); i++)
        {
            if (comps[i].size() < 41)
            {

                comps.erase(comps.begin() + i);
                std::vector<std::vector<int>>(comps).swap(comps);
                i = i - 1;
            }

        }

    // calculating the  average width and height of connectivity components
        for (int i = 0; i < comps.size(); i++)
        {

            max_j = comps[i][2];
            min_j = comps[i][2];
            min_i = comps[i][1];
            max_i = comps[i][1];
            for (int j = 2; j < comps[i].size(); j +=2)
            {

                if (comps[i][j] > max_j)
                    max_j = comps[i][j];
                if (comps[i][j] < min_j)
                    min_j = comps[i][j];
                if (comps[i][j-1] > max_i)
                    max_i = comps[i][j-1];
                if (comps[i][j-1] < min_i)
                    min_i = comps[i][j-1];

            }
            widths.push_back(max_j - min_j + 1);
            lengths.push_back(max_i - min_i + 1);

        }

        for (int i = 0; i < widths.size(); i++)
        {
            ratio.push_back(((double)widths[i])/(double)lengths[i]);
        }
        average_length = vecMed(widths);
        average_width = vecMed(widths);
        average_ratio = vecMed(ratio);


    // 2. IF ( WIDTH[I] > 1.5*AVERAGE WIDTH )  OR  IF (HEIGHT[I] > 1.5 * AVERAGE HEIGHT)||(HEIGHT[I] < 0.5 HEIGHT) , WE DON'T CONSIDER IT
        for (int i = 0; (i < widths.size())&&(i<lengths.size()); i++)
        {
            p = 0;
            if (widths[i] > 1.5 * average_width)
            {
                comps.erase(comps.begin() + i);
                std::vector<std::vector<int>>(comps).swap(comps);
                widths.erase(widths.begin() + i);
                std::vector<int>(widths).swap(widths);
                lengths.erase(lengths.begin() + i);
                std::vector<int>(lengths).swap(lengths);
                ratio.erase(ratio.begin() + i);
                std::vector<double>(ratio).swap(ratio);
                i = i - 1;
                p = 1;
            }
            if ((p !=1)&&((lengths[i] > 1.2 * average_length)|| (lengths[i] < 0.5 * average_length)))
            {
                comps.erase(comps.begin() + i);
                std::vector<std::vector<int>>(comps).swap(comps);
                widths.erase(widths.begin() + i);
                std::vector<int>(widths).swap(widths);
               lengths.erase(lengths.begin() + i);
                std::vector<int>(lengths).swap(lengths);
                ratio.erase(ratio.begin() + i);
                std::vector<double>(ratio).swap(ratio);
                i = i - 1;
                p=2;
            }
            if ((p!=1)&&(p!=2)&&((ratio[i] > 5*average_ratio)||(ratio[i] < 0.5*average_ratio)))
            {
                comps.erase(comps.begin() + i);
                std::vector<std::vector<int>>(comps).swap(comps);
                widths.erase(widths.begin() + i);
                std::vector<int>(widths).swap(widths);
               lengths.erase(lengths.begin() + i);
                std::vector<int>(lengths).swap(lengths);
                ratio.erase(ratio.begin() + i);
                std::vector<double>(ratio).swap(ratio);
                i = i - 1;
            }

        }

    //painting framing rectangles
        for (int i = 0; i < comps.size(); i++)
        {

            max_j = comps[i][2];
            min_j = comps[i][2];
            min_i = comps[i][1];
            max_i = comps[i][1];
            for (int j = 2; j < comps[i].size(); j += 2)
            {

                if (comps[i][j] > max_j)
                    max_j = comps[i][j];
                if (comps[i][j] < min_j)
                    min_j = comps[i][j];
                if (comps[i][j - 1] > max_i)
                    max_i = comps[i][j - 1];
                if (comps[i][j - 1] < min_i)
                    min_i = comps[i][j - 1];

            }
            for (int index1 = min_i; index1 <= max_i; index1++)
            {
                makeBlue(img, index1, min_j);
                makeBlue(img, index1, max_j);
            }
            for (int index1 = min_j; index1 <= max_j; index1++)
            {
                makeBlue(img, min_i, index1);
                makeBlue(img, max_i, index1);
            }

        }

    //calculation of the statistics
        for (int i = 0; i < comps.size(); i++)
        {
            dens.push_back(comps[i].size()/(widths[i] * lengths[i]));
        }

        help1 = 0; help2 = 0;help3 =0;
        for (int i = 0; i < widths.size(); i++)
        {
            help1 = help1 + widths[i];
            help2 = help2 + lengths[i];
            help3 = help3 + dens[i];
        }
        average_length = help1 / (widths.size());
        average_width = help2 / (lengths.size());
        average_dens = help3/(dens.size());

       max_width = max_element(widths.begin(), widths.end()) - widths.begin();
       min_width = min_element(widths.begin(), widths.end()) - widths.begin();

       max_length = max_element(lengths.begin(), lengths.end()) - lengths.begin();
       min_length = min_element(lengths.begin(), lengths.end()) - lengths.begin();

       max_dens = max_element(dens.begin(), dens.end()) - dens.begin();
       min_dens = min_element(dens.begin(), dens.end()) - dens.begin();
    //end of calculations


std::cout<<average_ratio<<std::endl;

for (int i = 0; i < widths.size(); i++)
{
    std::cout<<ratio[i]<<std::endl;
}
//    for (int k = 1; k < comps[max_dens].size(); k += 2)
//        makeBlack(img, comps[max_dens][k], comps[max_dens][k + 1]);
//   for (int k = 0; k < comps.size(); k++)
//           makeYellow(img, comps[k][1], comps[k][2]);
    imshow("Display Window", img);
    cv::waitKey(0);

   return 0;
}
