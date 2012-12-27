#include <iostream>
#include <stdio.h>

#include "opencv2/highgui/highgui.hpp"

#include <X11/Xlib.h>
#include <assert.h>

#include <mp_tracker/video_tracker.h>

VideoTracker::VideoTracker(){
  init();
}

/** @function init */
int VideoTracker::init(){

/*  //Xlib
  Display *dpy = XOpenDisplay((0));
  assert(dpy);
  int blackColor = BlackPixel(dpy, DefaultScreen(dpy));
  int whiteColor = WhitePixel(dpy, DefaultScreen(dpy));
  // Create the window
  Window w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, 
                                 200, 100, 0, blackColor, blackColor);
  // We want to get MapNotify events
  XSelectInput(dpy, w, StructureNotifyMask);
  // "Map" the window (that is, make it appear on the screen)
  XMapWindow(dpy, w);
  // Create a "Graphics Context"
  GC gc = XCreateGC(dpy, w, 0, (0));
  // Tell the GC we draw using the white color
  XSetForeground(dpy, gc, whiteColor);
  // Wait for the MapNotify event
  for(;;) {
    XEvent e;
    XNextEvent(dpy, &e);
    if (e.type == MapNotify)
    break;
  }
  // Draw the line
  XDrawLine(dpy, w, gc, 10, 60, 180, 20);
  // Send the "DrawLine" request to the server
  XFlush(dpy);
  // Wait for 10 seconds
  sleep(10);
*/


  face_cascade_name_ = "haarcascade_frontalface_alt.xml";
  //eyes_cascade_name_ = "haarcascade_eye_tree_eyeglasses.xml";
  eyes_cascade_name_ = "parojos.xml";
  
  window_name_ = "Capture - Face detection";
  CvCapture *capture;
  cv::namedWindow(window_name_,1);
  // -- 1. Load the cascades
  if (!face_cascade_.load(face_cascade_name_)) { printf("--(!)Error loading\n");
                                               return -1; }

  if (!eyes_cascade_.load(eyes_cascade_name_)) { printf("--(!)Error loading\n");
                                               return -1; }

  // -- 2. Read the video stream
  capture = cvCaptureFromCAM(-1);

  if (capture) {
    while (true){
      orig_frame_ = cvQueryFrame(capture);
      //cv::setMouseCallback(window_name_, &VideoTracker::onMouseCb, this);

      // -- 3. Apply the classifier to the frame
      if (!orig_frame_.empty()) 
        detectAndDisplay(); 
      else{ 
        printf(" --(!) No captured frame -- Break!"); 
        break; 
      }

      int c = cv::waitKey(10);

      if ((char)c == 'c') break;
    }
  }
  return 0;
}

/** @function detectAndDisplay */
std::vector<cv::Rect> VideoTracker::detectAndDisplay(int display=1){

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
    
    std::cout << "Face center is at: " << center << std::endl;

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
  
  if(display){
    // -- Show what you got
    drawPoints(window_name_, orig_frame_);
    cv::imshow(window_name_, orig_frame_);
  }

  return faces;
}

void VideoTracker::onMouseCb(int event, int x, int y, int flags, void *param) {
  VideoTracker* vt = static_cast<VideoTracker*>(param);
  if (event == CV_EVENT_LBUTTONDOWN) {
    cv::Point mpoint(x, y);
    //cv::Mat frame = (cv::Mat&) param;
    //imshow(window_name, frame);
    vt->points_.push_back(mpoint);
    return;
  } else if ((event == CV_EVENT_RBUTTONDOWN)) {
  } else {
    return;
  }
}

void VideoTracker::drawPoints( std::string window_name, cv::Mat & frame){
  for(size_t i=0; i<points_.size(); i++){
    cv::circle(frame, points_[i], 3, cv::Scalar(0, 0, 255), 1, 8);
  }
}
