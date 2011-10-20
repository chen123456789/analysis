#ifndef TotalEnergyDepositionTRDvsTrackerPlot_hh
#define TotalEnergyDepositionTRDvsTrackerPlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

class TotalEnergyDepositionTRDvsTrackerPlot : public AnalysisPlot, public H2DPlot
{

public:
  TotalEnergyDepositionTRDvsTrackerPlot();
  ~TotalEnergyDepositionTRDvsTrackerPlot();

  virtual void processEvent(const AnalyzedEvent*);
};

#endif /* TotalEnergyDepositionTRDvsTrackerPlot_hh */
