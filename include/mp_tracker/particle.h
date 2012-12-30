#ifndef PARTICLE_H
#define PARTICLE_H

#include <opencv2/opencv.hpp>

class Particle{
	public:
		Particle(){
			weight = 0;
			pos[0] = 0;
			pos[1] = 0;
			vel[0] = 0;
			vel[1] = 0;
			age = 0;
		};
		cv::Vec2d pos;    // Position of the particle x,y,theta
		cv::Vec2d vel;    // Velocity of the particle
		int age;          // Current age of the particle
		double weight;    // Weight of the particle
		bool operator ()(Particle const& a, Particle const& b) const {
		  return a.weight>b.weight;
		}
};
#endif
