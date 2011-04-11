#ifndef ParticleSelection_hh
#define ParticleSelection_hh

#include <QFlags>
#include <QMap>

class Particle;

class ParticleInformation {
public:
  ParticleInformation(const Particle* particle);
  ~ParticleInformation();
  
  enum Flag {None=0x0, AllTrackerLayers=0x1<<0, InsideMagnet=0x1<<1, OutsideMagnet=0x1<<2, HighTransverseRigidity=0x1<<3, 
    MagnetCollision=0x1<<4, Albedo=0x1<<5, Chi2Good=0x1<<6};

  Q_DECLARE_FLAGS(Flags, Flag);

public:
  void process();
  void reset();
  const Flags& flags() const {return m_flags;}
  const QMap<double,int>& hitsInLayers() const {return m_hitsInLayers;}
  int numberOfTrackerLayers() const {return m_hitsInLayers.size();}

private:
  void checkAllTrackerLayers();
  void checkChi2Good();
  void checkInsideMagnet();
  void checkOutsideMagnet();
  void checkHighTransverseRigidity();
  void checkMagnetCollision();
  void checkAlbedo();

private:
  const Particle* m_particle;
  Flags m_flags;
  QMap<double, int> m_hitsInLayers;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(ParticleInformation::Flags);

#endif /* ParticleSelection_hh */