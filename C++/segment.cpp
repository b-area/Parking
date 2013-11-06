#include "opencv2/opencv.hpp"
#include <string>

using namespace cv;
using namespace std;

int thresh = 100;
int max_thresh = 255;
int n_erode = 3;
int n_dilate = 3;
int max_erode = 20;
int max_dilate = 20;
cv::Mat image;
cv::Mat src_gray;

/* Function Headers */
void thresh_callback(int, void* );

class WatershedSegmenter{
private:
    cv::Mat markers;
public:
    void setMarkers(cv::Mat& markerImage)
    {
        markerImage.convertTo(markers, CV_32S);
    }

    cv::Mat process(cv::Mat &image)
    {
        cv::watershed(image, markers);
        markers.convertTo(markers,CV_8U);
        return markers;
    }
};


int main(int argc, char* argv[])
{
    image = cv::imread(argv[1]);
    cv::cvtColor(image, src_gray, CV_BGR2GRAY);
   
    /// Create Window
    const char* source_window = "Source";
    namedWindow( source_window, WINDOW_AUTOSIZE );
    imshow( source_window, image);

    createTrackbar( " Threshold:", "Source", &thresh, max_thresh, thresh_callback );
    createTrackbar( " Erode:", "Source", &n_erode, max_erode, thresh_callback );
    createTrackbar( " Dilate:", "Source", &n_dilate, max_dilate, thresh_callback );
    thresh_callback( 0, 0 );
    waitKey(0);

    return 0;
}


void thresh_callback(int, void* )
{
    cv::Mat binary;

    cv::threshold(src_gray, binary, thresh, 255, THRESH_BINARY);

    // Eliminate noise and smaller objects
    cv::Mat fg;
    cv::erode(binary,fg,cv::Mat(),cv::Point(-1,-1),n_erode);
    //imshow("fg", fg);

    // Identify image pixels without objects
    cv::Mat bg;
    cv::dilate(binary,bg,cv::Mat(),cv::Point(-1,-1), n_dilate);
    cv::threshold(bg,bg,1, 128,cv::THRESH_BINARY_INV);
    //imshow("bg", bg);

    // Create markers image
    cv::Mat markers(binary.size(),CV_8U,cv::Scalar(0));
    markers= fg+bg;
    //imshow("markers", markers);

    // Create watershed segmentation object
    WatershedSegmenter segmenter;
    segmenter.setMarkers(markers);

    cv::Mat result = segmenter.process(image);
    result.convertTo(result,CV_8U);
    imshow("final_result", result);

}
