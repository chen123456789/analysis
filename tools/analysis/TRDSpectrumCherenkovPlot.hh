#ifndef TRDSpectrumCherenkovPlot_hh
#define TRDSpectrumCherenkovPlot_hh

#include "TRDSpectrumPlot.hh"

class TRDSpectrumCherenkovPlot :
  public TRDSpectrumPlot
{
  
public:
  enum CherenkovCut{None = -1, BothAbove, BothBelow, C1AboveC2Below, C1BelowC2Above};

public:
  TRDSpectrumCherenkovPlot(unsigned short = 0, TRDSpectrumType = TRDSpectrumCherenkovPlot::completeTRD, CherenkovCut cCut = None, double c1Limit = 200, double c2Limit = 200);
  ~TRDSpectrumCherenkovPlot();
  
  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);

private:
  const CherenkovCut m_cherenkovCut;
  const double m_cherenkov1Limit;
  const double m_cherenkov2Limit;
  
};

#endif /* TRDSpectrumCherenkovPlot_hh */
