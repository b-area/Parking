#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>

int main(int argc, char *argv[])
{
    cv::Mat frame;
    cv::Mat fg;
    cv::Mat blurred;
    cv::Mat thresholded;
    cv::Mat blob;
    cv::Mat bgmodel;
    cv::namedWindow("Frame");
    cv::namedWindow("Background Model");
    cv::VideoCapture cap(0);
    cv::BackgroundSubtractorMOG2 bgs;
    const int nmixtures = 3;
    const int history = 1000;
    const int varThresholdGen = 15;
    const bool bShadowDetection = false;
    const int nShadowDetection = 0;
    const float fTau = 0.5;

    cv::BackgroundSubtractorMOG2 bg (history,nmixtures,bShadowDetection);
    //cv::BackgroundSubtractorMOG2 bg; 
    //bg.nmixtures = 3;
    //bg.bShadowDetection = false;
    
    std::vector<std::vector<cv::Point> > contours;
    for(;;)
    {
        cap >> frame;
        cv::GaussianBlur(frame,blurred,cv::Size(3,3),0,0,cv::BORDER_DEFAULT);
        bgs.operator()(frame,fg);
        bgs.getBackgroundImage(bgmodel);
        cv::threshold(fg,thresholded,70.0f,255,CV_THRESH_BINARY);
        //you can change morphological close value (25) to another value to get a solid blob
        cv::Mat element(25,25,CV_8U,cv::Scalar(1));
        cv::morphologyEx(thresholded,thresholded,cv::MORPH_CLOSE,element);
        cv::findContours(thresholded,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
        cv::cvtColor(thresholded,blob,CV_GRAY2RGB);
        cv::drawContours(blob,contours,-1,cv::Scalar(255,255,255),CV_FILLED,8);
        int cmin= 500; //min connected contours
        int cmax= 10000; //max connected contours
        std::vector<std::vector<cv::Point> >::iterator itc=contours.begin();
        while (itc!=contours.end()) {
            if (itc->size() > cmin || itc->size() < cmax){
                std::vector<cv::Point> pts = *itc;
                cv::Mat pointsMatrix = cv::Mat(pts);
                cv::Scalar color( 0, 255, 0 );
                cv::Rect r0= cv::boundingRect(pointsMatrix);
                cv::rectangle(frame,r0,color,2);
                ++itc;
            }else{++itc;}
        }
        cv::imshow("Frame",frame);
        cv::imshow("Background Model",bgmodel);
        cv::imshow("Blob",blob);
        if(cv::waitKey(30) >= 0) break;
    }

    return 0;
}
