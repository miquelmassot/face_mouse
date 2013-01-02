#ifndef PARTICLE_H
#define PARTICLE_H

#include <opencv2/opencv.hpp>

class Particle{
	public:
		Particle(){
			w = 0;
			x = 0;
			y = 0;
			t = 0;
			vx = 0;
			vy = 0;
			age = 0;
		};
		double x;    // Position of the particle x,y,theta
		double y;
		double t;
		double vx; // speed of the particle
		double vy;
		int age;          // Current age of the particle
		double w;    // Weight of the particle
		bool operator ()(Particle const& a, Particle const& b) const {
		  return a.w>b.w;
		}
};
#endif
