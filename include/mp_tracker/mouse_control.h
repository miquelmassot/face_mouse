#ifndef MOUSE_CONTROL_H
#define MOUSE_CONTROL_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>

class MouseControl{
	public:
		int x;
		int y;
		MouseControl();
		void grabMouse();
		void ungrabMouse();
		void setMousePosition(int x, int y);
		int getMousePosition();

	private:
		int screen_num_;
		Display *dpy_;
		void init();
		
		
};
#endif