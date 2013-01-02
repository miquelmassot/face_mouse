#include <face_mouse/mouse_control.h>
#include <iostream>

int main(void){

	MouseControl mc;

	mc.getMousePosition();
	std::cout << "Mouse position is (" << mc.x << " , " << mc.y << " )." << std::endl;
	mc.grabMouse();
	mc.setMousePosition(100,100);
	mc.ungrabMouse();
	mc.getMousePosition();
	std::cout << "Mouse position is (" << mc.x << " , " << mc.y << " )." << std::endl;
	return 0;
}