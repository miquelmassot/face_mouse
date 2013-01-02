#include <iostream>
#include <stdio.h>

#include <face_mouse/particle_filter.h>


int main(int argc, char **argv){
  ParticleFilter pf(1000);

  pf.generateParticles();

  return 0;
}
