#include <assert.h>

#include <face_mouse/mouse_control.h>

MouseControl::MouseControl(){
  init();
}

void MouseControl::init(){
  // Save the working display and check that it's ok
  dpy_ = XOpenDisplay((NULL));
  assert(dpy_);
}

void MouseControl::grabMouse(){
  // Mouse grabbing allows the programmer to stop any mouse movement
  // from the user. Needs ungrabbing to return the control to enduser!
  Screen *screen;
  if(getMousePosition()){
	  screen = ScreenOfDisplay(dpy_, screen_num_);
	  XGrabPointer(dpy_, screen->root, true, ButtonPressMask | ButtonReleaseMask | 
	      PointerMotionMask | FocusChangeMask | EnterWindowMask | LeaveWindowMask,
	      GrabModeAsync,GrabModeAsync, None, None, CurrentTime);
  }
}

void MouseControl::ungrabMouse(){
  // Undoes mouse grabbing.
  XUngrabPointer(dpy_, CurrentTime);
}

void MouseControl::setMousePosition(int x_in, int y_in){
  // Warps mouse position to the desired location in the screen
  Window screen_root = RootWindow(dpy_, screen_num_);
  XWarpPointer(dpy_, None, screen_root, 0, 0, 0, 0, x_in, y_in);
  // Flush is needed in order to refresh the screen and the mouse to
  // the desired position!
  XFlush(dpy_);
}

int MouseControl::getMousePosition(){
  // Saves actual mouse position in this.x and this.y
  // and returns 1 if has found the mouse, 0 if it's lost...
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
