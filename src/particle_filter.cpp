#include <algorithm> // sort function
#include <X11/Xlib.h>  // X11 library headers
#include <iostream>
#include <cstdlib> //random numbers
#include <assert.h>

#include <face_mouse/particle_filter.h>
#include <face_mouse/video_tracker.h>

//using namespace srv;
const double PI = std::atan(1.0)*4;

ParticleFilter::ParticleFilter(int n){
  init(n);
}

void ParticleFilter::init(int n){
  //TODO change seed
  srand(0);
}

void ParticleFilter::generateParticles(void){
  // get the properties of the camera frame in order to generate the particles
  std::vector<int> resolution = vt_.getResolution();
  double minx = 0;
  double miny = 0;
  double maxx = (double) resolution[0];
  double maxy = (double) resolution[1];
  double mint = 0;
  double maxt = PI;
  size_t N = p_.size();
  for(size_t i=0; i<N; i++){
    p_[i].x = minx + ((double)rand()/((double)RAND_MAX+1.0))*maxx;
    p_[i].y = miny + ((double)rand()/((double)RAND_MAX+1.0))*maxy;
    p_[i].t = mint + ((double)rand()/((double)RAND_MAX+1.0))*maxt;
    p_[i].vx = 0;
    p_[i].vy = 0;
    p_[i].age = 0;
    p_[i].w = 0;
  }
}
void ParticleFilter::generateParticles(double minx, double maxx, 
                                       double miny, double maxy, 
                                       double mint, double maxt){
  size_t N = p_.size();
  for(size_t i=0; i<N; i++){
    p_[i].x = minx + ((double)rand()/((double)RAND_MAX+1.0))*maxx;
    p_[i].y = miny + ((double)rand()/((double)RAND_MAX+1.0))*maxy;
    p_[i].t = mint + ((double)rand()/((double)RAND_MAX+1.0))*maxt;
    p_[i].vx = 0;
    p_[i].vy = 0;
    p_[i].age = 0;
    p_[i].w = 0;
  }
}

Particle ParticleFilter::getBestParticle(void){
  // use best 10% particles to return output
  sortParticles();
  size_t N = p_.size();
  int best10 = (int)(0.9*(double)N);
  Particle p;
  for (size_t i=best10; i<N; i++){
    p.x += p_[i].x;
    p.y += p_[i].y;
    p.t += p_[i].t;
    p.vx += p_[i].vx;
    p.vy += p_[i].vy;
    p.w += p_[i].w;
  }
  p.x /= (N-best10);
  p.y /= (N-best10);
  p.t /= (N-best10);
  p.vx /= (N-best10);
  p.vy /= (N-best10);
  p.w /= (N-best10);
  return p;
}

void ParticleFilter::getMeasurements(void){
  //TODO use VideoTracker
  // and dont do a while(1) ! 

  vt_.step();

  // suppose there's only one face?
  // i'll get only the first
  
  m_.x = vt_.faces[0].x + vt_.faces[0].width/2;
  m_.y = vt_.faces[0].y + vt_.faces[0].height/2;

}

void ParticleFilter::predict(void){
  //TODO use the particle velocity and move
  //the particle position according to vel
  //plus some random noise.
  size_t N = p_.size();
  for(size_t i=0; i<N; i++){
    double noisex = ((double) rand() / ((double)RAND_MAX+1.0));
    double noisey = ((double) rand() / ((double)RAND_MAX+1.0));
    double noisevx = ((double) rand() / ((double)RAND_MAX+1.0));
    double noisevy = ((double) rand() / ((double)RAND_MAX+1.0));
    p_[i].x += p_[i].vx + noisex;
    p_[i].y += p_[i].vy + noisey;
    p_[i].vx += noisevx;
    p_[i].vy += noisevy;
  }

}

double ParticleFilter::evaluateObservation(int i){
  double weight = 0;

  double ex = p_[i].x - m_.x;
  double ey = p_[i].x - m_.y;

  weight = sqrt(ex*ex + ey*ey);

  return 1/weight;
}

void ParticleFilter::weightParticles(void){
  size_t N = p_.size();
  double cum_weight = 0;
  // Calculate the error between observation and prediction
  // and obtain weights
  for(size_t i=0; i<N; i++){
    p_[i].w =  evaluateObservation(i);
    p_[i].age++;
    cum_weight = p_[i].w;
  }
  // normalize weights so that sum(p_.weight) = 1.0
  for(size_t i=0; i<N; i++)
    p_[i].w /= cum_weight;
}

void ParticleFilter::sortParticles(void){
  std::sort(p_.begin(), p_.end(), Particle());
}

std::vector<Particle> ParticleFilter::systematicResample(void){
  size_t N = p_.size();
  //generate random step number
  double rnd =((double) rand() / ((double)RAND_MAX+1.0));
  double step = 1.0/N * rnd;
  // create an incremental weight vector
  std::vector<int> c(N);
  c[1] = p_[1].w;
  for(size_t i=1; i<N; i++){
    c[i] = c[i+1] + p_[i].w;
  }
  std::vector<Particle> out(N);

  size_t i = 0;
  size_t j = 0;
  // select particles that are pointed by step in vector c
  while (j<N && step<1){
    if (i==1 && step < c[i]){
      out[j] = p_[i+1];
      j++;
      step = step + 1.0/N;
    }else if (step >= c[i] && step < c[i + 1]){
      out[j] = p_[i];
      j++;
      step = step + 1.0/N;
    }else if (i<N-1){
      i++;
    }
  }
  // return out particle vector
  return out;
}

void ParticleFilter::run(void){
  getMeasurements();
  predict();
  weightParticles();
  std::vector<Particle> p(p_.size());
  p = systematicResample();
}
