#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    cv::RNG rng(12345);
    
    cv::Mat frame;
    cv::Mat back;
    cv::Mat fore;
    cv::VideoCapture cap(0);

    const int nmixtures =3;
    const bool bShadowDetection = false;
    const int history = 4;
    cv::BackgroundSubtractorMOG2 bg (history,nmixtures,bShadowDetection);
    //cv::BackgroundSubtractorMOG2 bg; 
    //bg.nmixtures = 3;
    //bg.bShadowDetection = false;
 
    std::vector<std::vector<cv::Point> > contours;
 
    cv::namedWindow("Frame");
    cv::namedWindow("Background");
 
    for(;;)
    {
        cap >> frame;
        bg.operator ()(frame,fore);
        bg.getBackgroundImage(back);
        cv::erode(fore,fore,cv::Mat());
        cv::dilate(fore,fore,cv::Mat());
        cv::findContours(fore,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE); // CV_RETR_EXTERNAL retrieves only the extreme outer contours
        //cv::drawContours(frame,contours,-1,cv::Scalar(0,0,255),2); // This draws all the contours
        // Only draw important contours
        for( int i = 0; i< contours.size(); i++ )
        {
            cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
            //cv::drawContours(frame, contours, i, color, 2, 8, hierarchy, 0, Point() );
            double area = contourArea(contours[i]);
            if (area > 75.0)
            {
                printf("Area[%d]: %.2f\n", i, contourArea(contours[i]));
                cv::drawContours(frame,contours,i,color,2); // This draws all the contours
            }
        }
        
        cv::imshow("Frame",frame);
        cv::imshow("Background",back);
        if(cv::waitKey(30) >= 0) break;
    }
    return 0;
}
