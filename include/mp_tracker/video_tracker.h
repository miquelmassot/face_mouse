#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

class VideoTracker{

  public:
    /** Function Headers */
    VideoTracker(void);
    void detectAndDisplay();

  protected:
    cv::String face_cascade_name_;
    cv::String eyes_cascade_name_;
    std::string window_name_;
    cv::CascadeClassifier face_cascade_;
    cv::CascadeClassifier eyes_cascade_;
    std::vector<cv::Point2d> points_;
    cv::Mat frame_;
    cv::Mat orig_frame_;

  private:
    int init(void);
    static void onMouseCb(int event, int x, int y, int flags, void *param);
    void drawPoints( std::string window_name, cv::Mat & frame);

};
