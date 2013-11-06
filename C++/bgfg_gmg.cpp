/*
 * FGBGTest.cpp
 *
 *  Created on: May 7, 2012
 *      Author: Andrew B. Godbehere
 */


#include <opencv2/video/video.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

/*
#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <opencv/highgui.h>
#include <iostream>
*/

 

using namespace cv;

static void help()
{
    std::cout <<
    "\nA program demonstrating the use and capabilities of a particular BackgroundSubtraction\n"
    "algorithm described in A. Godbehere, A. Matsukawa, K. Goldberg, \n"
    "\"Visual Tracking of Human Visitors under Variable-Lighting Conditions for a Responsive\n"
    "Audio Art Installation\", American Control Conference, 2012, used in an interactive\n"
    "installation at the Contemporary Jewish Museum in San Francisco, CA from March 31 through\n"
    "July 31, 2011.\n"
    "Call:\n"
    "./BackgroundSubtractorGMG_sample\n"
    "Using OpenCV version " << CV_VERSION << "\n"<<std::endl;
}

int main(int argc, char** argv)
{
    help();

    initModule_video();
    setUseOptimized(true);
    setNumThreads(8);

    //Ptr<BackgroundSubtractor> fgbg = createBackgroundSubtractorMOG(20, 0.7);
    //BackgroundSubtractorMOG2
    Ptr<BackgroundSubtractor> fgbg = new BackgroundSubtractorMOG(3, 4, 0.8);

    if (fgbg.empty())
    {
        std::cerr << "Failed to create BackgroundSubtractor.GMG Algorithm." << std::endl;
        return -1;
    }

    VideoCapture cap;
    if (argc > 1)
        cap.open(argv[1]);
    else
        cap.open(0);

    if (!cap.isOpened())
    {
        std::cerr << "Cannot read video. Try moving video file to sample directory." << std::endl;
        return -1;
    }

    Mat frame, fgmask, segm;

    namedWindow("FG Segmentation", WINDOW_NORMAL);

    for (;;)
    {
        cap >> frame;

        if (frame.empty())
            break;

         fgmask = fgbg->apply(frame);

        frame.convertTo(segm, CV_8U, 0.5);
        add(frame, Scalar(100, 100, 0), segm, fgmask);

        imshow("FG Segmentation", segm);

        int c = waitKey(30);
        if (c == 'q' || c == 'Q' || (c & 255) == 27)
            break;
    }

    return 0;
}

