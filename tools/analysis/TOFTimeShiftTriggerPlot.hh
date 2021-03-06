#ifndef TOFTimeShiftTriggerPlot_hh
#define TOFTimeShiftTriggerPlot_hh

#include "AnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

class TOFTimeShiftTriggerPlot : public AnalysisPlot, public H2DProjectionPlot {
public:
  TOFTimeShiftTriggerPlot();
  virtual ~TOFTimeShiftTriggerPlot();
  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  virtual void finalize();
  virtual void draw(TCanvas* canvas);
};

#endif
