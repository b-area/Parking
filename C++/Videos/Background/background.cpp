#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include <stdio.h>
#include <stdlib.h>

int parking_count = 0;
bool traceParking = false;

std::vector<cv::Point> parking; // polygo that enclose a parking space;

/*
 * Drawing ang getting the rect (polygon) that
 * encircle a parking space.
 */
void onMouse( int event, int x, int y, int flags, void* param )
{
    switch( event )
    {
        case CV_EVENT_LBUTTONDOWN:
            if (traceParking)
            {
                parking.push_back(cv::Point(x,y));
            }
            break;
        case CV_EVENT_LBUTTONUP:
            break;
    } 
}


/*
 * Draw area that is enclosing the parking space.
 *
 */
void drawPaking(cv::Mat &img)
{
    // Draw parking space
    if (!traceParking)
    {
        //cv::drawContours(frame, parking, -1, cv::Scalar(0,0,255), 2);
        const cv::Point *pts = (const cv::Point*) cv::Mat(parking).data;
        int npts = cv::Mat(parking).rows;
        //std::cout << "Number of polygon vertices: " << npts << std::endl;
        // draw the polygon 
        polylines(img, &pts,&npts, 1,
                true,           // draw closed contour (i.e. joint end to start) 
                cv::Scalar(0,255,0),// colour RGB ordering (here = green) 
                3,              // line thickness
                CV_AA, 0);
    }
}


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
    cv::setMouseCallback( "Frame", onMouse, 0 );
    //cv::createTrackbar( "Smin", "CamShift Demo", &smin, 256, 0 );

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
        for( unsigned int i = 0; i< contours.size(); i++ )
        {
            cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
            //cv::drawContours(frame, contours, i, color, 2, 8, hierarchy, 0, Point() );
            double area = contourArea(contours[i]);
            if (area > 215.0)
            {
                //printf("Area[%d]: %.2f\n", i, contourArea(contours[i]));
                cv::drawContours(frame,contours,i,color,2); // This draws all the contours

                // Track if an object of interest has crossed a roi
                // approxPolyDP(const Mat& curve, vector<Point>& approxCurve, double epsilon, bool closed)
                // double pointPolygonTest(const Mat& contour, Point2f pt, bool measureDist)

                cv::Rect box = cv::boundingRect(contours[i]); // Get bounding box around a countour
                // std::cout << "[x=" << box.x << " , y=" << box.y << ",w=" << box.width << ",h=" << box.height << "]\n"; 
                cv::Point2f center(box.x + box.width/2.0, box.y + box.height/2.0);

                // Test if the center of a contour has crossed ROI (direction: going in or out)
                double dist2Center = pointPolygonTest(contours[i], center, true);
                //std::cout << center << "is " << dist2Center << " distance from the contour. \n"; 
                cv::putText(frame, "I", center, cv::FONT_HERSHEY_COMPLEX_SMALL, 1.5, color, 1);
                cv::rectangle(frame, box.tl(), box.br(), cv::Scalar(100, 100, 200), 2, CV_AA);
            }
        }


        /*
         * Draw parking zone
         */
        for (unsigned int j = 0; j < parking.size(); j++)
        {
            cv::circle(frame, parking[j], 5, cv::Scalar(0,0,255), -1);
        }
        drawPaking(frame);

        cv::imshow("Frame",frame);
        cv::imshow("Background",back);

        char c = (char)cv::waitKey(30);
        if( c == 27 || c == 'q')
            break;
        switch(c)
        {
            case 's':
                traceParking = !traceParking; 
                break;
            case 'c':
                parking.clear();
                break;
            default:
                ;
        }
    }       

    return 0;
}
