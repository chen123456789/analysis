#ifndef MultiLayerTrackingEfficiencyPlot_hh
#define MultiLayerTrackingEfficiencyPlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

class TH1D;
class TCanvas;

class MultiLayerTrackingEfficiencyPlot :
  public AnalysisPlot,
  public H2DPlot
{
  
public:
  MultiLayerTrackingEfficiencyPlot();
  ~MultiLayerTrackingEfficiencyPlot();
  
  void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  void draw(TCanvas*);
  void finalize();

private:
  TH1D* m_normHisto;
  int m_nLayers;
  double* m_layerZ;
  
};

#endif /* MultiLayerTrackingEfficiencyPlot_hh */