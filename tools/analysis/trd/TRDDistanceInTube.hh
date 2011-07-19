#ifndef TRDDISTANCEINTUBE_HH
#define TRDDISTANCEINTUBE_HH

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TRDDistanceInTube : public AnalysisPlot, public H1DPlot
{

public:
  TRDDistanceInTube(AnalysisPlot::Topic);
  ~TRDDistanceInTube();

  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  virtual void finalize();

};

#endif // TRDDISTANCEINTUBE_HH
