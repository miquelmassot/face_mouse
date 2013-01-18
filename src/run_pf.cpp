#include <iostream>
#include <stdio.h>

#include <face_mouse/particle_filter.h>


int main(int argc, char **argv){
  ParticleFilter pf(1000);

  std::cout << "Particles generated!" << std::endl;

  pf.getMeasurements();
  
  std::cout << "Measurements ready!" << std::endl;
  
  pf.predict();
  
  std::cout << "Prediction made!" << std::endl;
  
  pf.weightParticles();
  
  std::cout << "Particles weighted!" << std::endl; //There are no particles in vector p??
  
  std::vector<Particle> p(pf.p_.size());
  
  std::cout << pf.p_.size() << std::endl;
  
  std::cout << "New Particles vector generated!" << std::endl;
  

  p = pf.systematicResample();

  std::cout << "Particles resampled!" << std::endl;
  
  return 0;
}
