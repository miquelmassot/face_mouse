

struct Particle{
  cv::Vec2d pos;  // Position of the particle x,y,theta
  cv::Vec2d vel; // Velocity of the particle
  cv::Vec3b color; // Color of the particle
  int age;          // Current age of the particle
  double weight;         // Weight of the particle
};

class ParticleFilter{

  public:
    ParticleFilter(int n);
    run();

  protected:

  private:
    std::vector<Particle> p_; // particle vector
    cv::Mat Pz;
    cv::Vec3d m_; // measurement

    void init(int n);
    void generateParticles();//done
    void getMeasurements();
    void predict(); //done
    void getBestParticle(void){
    bool compareParticles(Particle p1, Particle p2);
    void sortParticles(void);
    void weightParticles(); //done
    std::vector<Particle> systematicResample(); //done
    void evaluateObservation(int i);

};
