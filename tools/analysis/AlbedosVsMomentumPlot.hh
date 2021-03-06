#ifndef AlbedosVsMomentumPlot_hh
#define AlbedosVsMomentumPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TH1D;

class AlbedosVsMomentumPlot :
  public AnalysisPlot,
  public H1DPlot
{
  
public:
  AlbedosVsMomentumPlot();
  ~AlbedosVsMomentumPlot();
  
  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  void update();

private:
  TH1D* m_albedoHisto;
  TH1D* m_totalHisto;

};

#endif /* AlbedosVsMomentumPlot_hh */
