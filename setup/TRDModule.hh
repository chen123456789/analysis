#ifndef TRDModule_hh
#define TRDModule_hh

#include "DetectorElement.hh"

class TRDModule :
  public DetectorElement
{
  
public:
  TRDModule();
  TRDModule(unsigned short detId);
  ~TRDModule();
  
public:
  QVector<Cluster*> findClusters() const; // these clusters have to be deleted by hand!

};

#endif /* TRDModule_hh */