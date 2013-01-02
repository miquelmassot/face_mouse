#ifndef VIDEO_TRACKER_H
#define VIDEO_TRACKER_H

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"

class VideoTracker{

  public:
    /** Function Headers */
    std::vector<cv::Rect> faces;
    VideoTracker(void);
    VideoTracker(int display);
    std::vector<cv::Rect> detect();
    int step();
    int loop();
    void drawPoint(int x, int y);
    void drawRectangle(int x, int y, int width, int height);
    std::vector<int> getResolution(void);
  protected:
    cv::Mat frame_;
    cv::Mat orig_frame_;

  private:
    cv::String face_cascade_name_;
    cv::String eyes_cascade_name_;
    std::string window_name_;
    cv::CascadeClassifier face_cascade_;
    cv::CascadeClassifier eyes_cascade_;
    cv::VideoCapture stream_;
    int display_;
    int init(void);

};
#endif
