#include <iostream>
#include <stdio.h>

#include <mp_tracker/mouse_control.h>
#include <mp_tracker/video_tracker.h>

/** @function main */
int main(int argc, const char **argv){
  VideoTracker vt(1);
  MouseControl mc;
  //vt.loop();
  
  int orig_x,orig_y;
  //mc.grabMouse();

  int scale = 1;
  int mov_x,mov_y;
  int mx,my;

  int dead_zone_x = 30;
  int dead_zone_y = 15;

  // initialization
  while(vt.step()){
   if(vt.faces.size()>0){
    orig_x = vt.faces[0].x;
    orig_y = vt.faces[0].y;
    //TODO vt.drawPoint(orig_x,orig_y);
    break;
   }
  }

  // run continuously
  while(vt.step()){
   if(vt.faces.size()>0){
      mov_x = 0;
      mov_y = 0;
      mov_x = vt.faces[0].x - orig_x;
      mov_y = vt.faces[0].y - orig_y;
      if(((mov_x > dead_zone_x)||(-mov_x > dead_zone_x))||
          ((mov_y > dead_zone_y)||(-mov_y > dead_zone_y))){
        mc.getMousePosition();
        mx = mc.x + scale*mov_x;
        my = mc.y + scale*mov_y;
        std::cout << "Moving the mouse to ( " 
          << mx  << " , " << my << " )" << std::endl;
        mc.setMousePosition(mx, my);
      }
    }
  }

  //mc.ungrabMouse();

  return 0;

}
