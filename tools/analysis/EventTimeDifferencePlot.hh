#ifndef EventTimeDifferencePlot_hh
#define EventTimeDifferencePlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class EventTimeDifferencePlot : public AnalysisPlot, public H1DPlot {
public:
  EventTimeDifferencePlot(int numberOfThreads);
  virtual ~EventTimeDifferencePlot();
  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  void update();
  void finalize();
private:
  double exponentialDistribution(double*, double*);
  double m_lastEventTime;
  bool m_active;
};

#endif
