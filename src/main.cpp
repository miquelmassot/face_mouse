#include <iostream>
#include <stdio.h>

#include <mp_tracker/mouse_control.h>
#include <mp_tracker/video_tracker.h>

/** @function main */
int main(int argc, const char **argv){
  VideoTracker vt(1);
  MouseControl mc;
  //vt.loop();
  
  int x_face,y_face;
  //mc.grabMouse();

  int scale = 1;
  int mov_x,mov_y;
  int mx,my;

  int dead_zone_x = 30;
  int dead_zone_y = 15;

  // initialization
  while(vt.step()){
   if(vt.faces.size()>0){
    x_face = vt.faces[0].x + vt.faces[0].width/2;
    y_face = vt.faces[0].y + vt.faces[0].height/2;
    vt.drawPoint(x_face,y_face);
    vt.drawRectangle(x_face - dead_zone_x/2, y_face-dead_zone_y/2, 
        dead_zone_x, dead_zone_y);
    break;
   }
  }
  
  sleep(10);


  // run continuously
  while(vt.step()){
    vt.drawPoint(x_face,y_face);
    vt.drawRectangle(x_face - dead_zone_x/2, y_face-dead_zone_y/2, 
        dead_zone_x, dead_zone_y);
    if(vt.faces.size()>0){
      int new_x_face = vt.faces[0].x + vt.faces[0].width/2;
      int new_y_face = vt.faces[0].y + vt.faces[0].height/2;
      vt.drawPoint(new_x_face,new_y_face);
      mov_x = new_x_face - x_face;
      mov_y = new_y_face - y_face;
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
