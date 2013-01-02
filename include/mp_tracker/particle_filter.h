#ifndef PARTICLE_FILTER_H
#define PARTICLE_FILTER_H

#include "opencv2/imgproc/imgproc.hpp"

#include <mp_tracker/particle.h>

//namespace srv{
class ParticleFilter{
  public:
    ParticleFilter(int n);
    void run(void);

  protected:
    std::vector<Particle> p_; // particle vector
    cv::Mat Pz;
    cv::Point m_; // measurement

    void generateParticles(double minx, double maxx, double miny, double maxy, double mint, double maxt);//done
    void getMeasurements(void);
    void predict(void); //done
    Particle getBestParticle(void);
    void sortParticles(void);
    void weightParticles(void); //done
    std::vector<Particle> systematicResample(void); //done
    double evaluateObservation(int i);
    void init(int n);
};
//};
#endif
