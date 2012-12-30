#include <assert.h>

#include <mp_tracker/mouse_control.h>

MouseControl::MouseControl(){
  init();
}

void MouseControl::init(){
  dpy_ = XOpenDisplay((NULL));
  assert(dpy_);
}

void MouseControl::grabMouse(){
  Screen *screen;
  if(getMousePosition()){
	  screen = ScreenOfDisplay(dpy_, screen_num_);
	  XGrabPointer(dpy_, screen->root, true, ButtonPressMask | ButtonReleaseMask | 
	      PointerMotionMask | FocusChangeMask | EnterWindowMask | LeaveWindowMask,
	      GrabModeAsync,GrabModeAsync, None, None, CurrentTime);
  }
}

void MouseControl::ungrabMouse(){
  XUngrabPointer(dpy_, CurrentTime);
}

void MouseControl::setMousePosition(int x_in, int y_in){
  Window screen_root = RootWindow(dpy_, screen_num_);
  XWarpPointer(dpy_, None, screen_root, 0, 0, 0, 0, x_in, y_in);
  XFlush(dpy_);
}

int MouseControl::getMousePosition(){
  int x_out=0;
  int y_out=0;
  int dummy_int=0;
  unsigned int dummy_uint = 0;
  Window dummy_win = 0;
  int ret = False;
  int screencount = ScreenCount(dpy_);
  for(int i=0; i<screencount; i++){
    Screen *screen = ScreenOfDisplay(dpy_, i);
    ret = XQueryPointer(dpy_, RootWindowOfScreen(screen),
                        &dummy_win, &dummy_win,
                        &x_out, &y_out, &dummy_int, &dummy_int, &dummy_uint);
    if (ret == True) {
      screen_num_ = i;
      x = x_out;
      y = y_out;
      break;
    }
  }
  return ret;
}