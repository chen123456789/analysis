#ifndef TriggerRateTimePlot_hh
#define TriggerRateTimePlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QDateTime>

class TriggerRateTimePlot : public AnalysisPlot, public H1DPlot {
public:
  TriggerRateTimePlot(QDateTime first, QDateTime last);
  virtual ~TriggerRateTimePlot();
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  virtual void finalize();
};

#endif