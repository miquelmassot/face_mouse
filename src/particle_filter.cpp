#include <stdio>
#include <stdlib>
#include <algorithm>

ParticleFilter::ParticleFilter(int n){
  init(int n);
}

ParticleFilter::init(int n){
  //TODO change seed
  srand(0);

  //Xlib
  Display *dpy = XOpenDisplay((0));
  assert(dpy);
  int blackColor = BlackPixel(dpy, DefaultScreen(dpy));
  int whiteColor = WhitePixel(dpy, DefaultScreen(dpy));
  // Create the window
  Window w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, 
                                 200, 100, 0, blackColor, blackColor);
  // We want to get MapNotify events
  XSelectInput(dpy, w, StructureNotifyMask);
  // "Map" the window (that is, make it appear on the screen)
  XMapWindow(dpy, w);
  // Create a "Graphics Context"
  GC gc = XCreateGC(dpy, w, 0, (0));
  // Tell the GC we draw using the white color
  XSetForeground(dpy, gc, whiteColor);
  // Wait for the MapNotify event
  for(;;) {
    XEvent e;
    XNextEvent(dpy, &e);
    if (e.type == MapNotify)
      break;
  }
  // Draw the line
  XDrawLine(dpy, w, gc, 10, 60, 180, 20);
  // Send the "DrawLine" request to the server
  XFlush(dpy);
  // Wait for 10 seconds
  sleep(10);
}

ParticleFilter::generateParticles(int minx, int maxx, 
                                  int miny, int maxy, 
                                  int mint, int maxt){
  size_t N = p_.size();
  for(size_t i=0; i<N; i++){
    p_[i].pos[0] = minx + ((double)rand()/(RAND_MAX+1))*maxx;
    p_[i].pos[1] = miny + ((double)rand()/(RAND_MAX+1))*maxy;
    p_[i].pos[3] = mint + ((double)rand()/(RAND_MAX+1))*maxt;
    p_[i].age = 0;
    p_[i].weight = 0;
  }
  
}

Particle ParticleFilter::getBestParticle(void){
  // use best 10% particles to return output
  sortParticles();
  size_t N = p_.size();
  int best10 = (int)(0.9*(double)N);
  Particle p_best;
  for (size_t i=best10; i<N; i++){
    p.pos[0] += p_[i].pos[0];
    p.pos[1] += p_[i].pos[1];
    p.vel[0] += p_[i].vel[0];
    p.vel[1] += p_[i].vel[1];
    p.weight += p_[i].weight;
  }
  p.pos[0] /= (N-bes10);
  p.pos[1] /= (N-bes10);
  p.vel[0] /= (N-bes10);
  p.vel[1] /= (N-bes10);
  p.weight /= (N-bes10);

}

void ParticleFilter::getMeasurements(){
  //TODO use VideoTracker
  // and dont do a while(1) ! 
  VideoTracker vt;

  std::vector<cv::Rect> faces;
  faces = vt.detectAndDisplay(0);
  // suppose there's only one face?
  // i'll get only the first

}

void ParticleFilter::predict(){
  //TODO use the particle velocity and move
  //the particle position according to vel
  //plus some random noise.
  size_t N = p_.size();
  for(size_t i=0; i<N; i++){
    double noise = ((double) rand() / (RAND_MAX+1));
    p_[i].pos[0] = p_[i].vel[0] + noise;
    p_[i].pos[1] = p_[i].vel[1] + noise;
  }

}

void ParticleFilter::evaluateObservation(int i){

}

void ParticleFilter::weightParticles(){
  size_t N = p_.size();
  double cum_weight = 0;
  // Calculate the error between observation and prediction
  // and obtain weights
  for(size_t i=0; i<N; i++){
    p_[i].weight =  evaluateObservation(i);
    p_[i].age++;
    cum_weight = p_[i].weight;
  }
  // normalize weights so that sum(p_.weight) = 1.0
  for(size_t i=0; i<N; i++)
    p_[i].weight /= cum_weight;
}

bool ParticleFilter::compareParticles(Particle p1, Particle p2){
  return p1.weight>p2.weight;
}

void ParticleFilter::sortParticles(void){
  std::sort(p_.begin(), p_.end(), ParticleFilter::compareParticles);
}

std::vector<Particle> ParticleFilter::systematicResample(){
  size_t N = p_.size();
  //generate random step number
  double rnd =((double) rand() / (RAND_MAX+1));
  double step = 1.0/N * rnd;
  // create an incremental weight vector
  std::vector<int> c(N);
  c[1] = p_[1].weight;
  for(size_t i=1; i<N; i++){
    c[i] = c[i+1] + p_[i];
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

void ParticleFilter::run(){
  getMeasurements();
  predict();
  weightParticles();
  systematicResampling();
  regenerate();
}
