#include <iostream>
#include <stdio.h>

#include <X11/Xlib.h>
#include <assert.h>

#include <mp_tracker/video_tracker.h>

VideoTracker::VideoTracker(){
  display_ = 0;
  init();
}

VideoTracker::VideoTracker(int display){
  display_ = display;
  init();
}

/** @function init */
int VideoTracker::init(){

  face_cascade_name_ = "haarcascade_frontalface_alt.xml";
  //eyes_cascade_name_ = "haarcascade_eye_tree_eyeglasses.xml";
  eyes_cascade_name_ = "parojos.xml";
  
  // -- 1. Load the cascades
  if (!face_cascade_.load(face_cascade_name_)) { printf("--(!)Error loading\n");
                                               return -1; }

  if (!eyes_cascade_.load(eyes_cascade_name_)) { printf("--(!)Error loading\n");
                                               return -1; }

  if(display_){
    window_name_ = "Capture - Face detection";
    cv::namedWindow(window_name_,1);
  }

  stream_ = openStream();

  return 0;
}

CvCapture* VideoTracker::openStream(){
  // -- 2. Read the video stream
  CvCapture *capture;
  capture = cvCaptureFromCAM(-1);

  return capture;
}

int VideoTracker::step(){
  if (stream_) {
    orig_frame_ = cvQueryFrame(stream_);
    // -- 3. Apply the classifier to the frame
    if (!orig_frame_.empty()) 
      faces = detect();
    else{ 
      printf(" --(!) No captured frame -- Break!"); 
      return 0;
    }

    int c = cv::waitKey(10);

    if ((char)c == 'c') return 0;;
  }
  return 1;
}

int VideoTracker::loop(){
  while(step()){}
  return 0;
}

/** @function detectAndDisplay */
std::vector<cv::Rect> VideoTracker::detect(){

  std::vector<cv::Rect> faces;
  cv::Mat frame_gray;
  cv::Mat orig_frame_gray;

  cv::resize(orig_frame_, frame_, cv::Size(), 0.2, 0.2);
  
  cv::cvtColor(frame_, frame_gray, CV_BGR2GRAY);
  cv::equalizeHist(frame_gray, frame_gray);
  cv::cvtColor(orig_frame_, orig_frame_gray, CV_BGR2GRAY);
  cv::equalizeHist(orig_frame_gray, orig_frame_gray);

  // -- Detect faces
  face_cascade_.detectMultiScale(frame_gray,
                                faces,
                                1.1,
                                2,
                                0 | CV_HAAR_SCALE_IMAGE,
                                cv::Size(30, 30));

  for (size_t i = 0; i < faces.size(); i++){
    faces[i].x *= 5;
    faces[i].y *= 5;
    faces[i].width *= 5;
    faces[i].height *= 5;

    cv::Point center(faces[i].x + faces[i].width * 0.5,
                 faces[i].y + faces[i].height * 0.5);
    cv::ellipse(orig_frame_, center, cv::Size(faces[i].width * 0.5,
                                faces[i].height * 0.5), 0, 0, 360,
            cv::Scalar(255, 0, 255), 4, 8, 0);
    
    std::cout << "Face " << i+1 << " center is at: " << center << std::endl;

    cv::Mat faceROI = orig_frame_gray(faces[i]);
    std::vector<cv::Rect> eyes;

    // -- In each face, detect eyes
    eyes_cascade_.detectMultiScale(faceROI,
                                  eyes,
                                  1.1,
                                  2,
                                  0 | CV_HAAR_SCALE_IMAGE,
                                  cv::Size(10, 10));

    for (size_t j = 0; j < eyes.size(); j++){
      cv::Point center(faces[i].x + eyes[j].x + eyes[j].width * 0.5,
                   faces[i].y + eyes[j].y + eyes[j].height * 0.5);
      int radius = cvRound((eyes[j].width + eyes[j].height) * 0.25);
      cv::circle(orig_frame_, center, radius, cv::Scalar(255, 0, 0), 4, 8, 0);
    }
  }
  
  if(display_){
    // -- Show what you got
    cv::imshow(window_name_, orig_frame_);
  }

  return faces;
}

void VideoTracker::drawPoint(int x, int y){
  cv::circle(orig_frame_, cv::Point(x,y), 3, cv::Scalar(0,0,255), 2);
  cv::imshow(window_name_, orig_frame_);
  cv::waitKey(3);
}

void VideoTracker::drawRectangle(int x, int y, int width, int height){
  cv::Point pt1(x, y);
  cv::Point pt2(x+width, y+height);
  cv::rectangle(orig_frame_, pt1, pt2, cv::Scalar(0,0,255));
  cv::imshow(window_name_, orig_frame_);
  cv::waitKey(3);
}
