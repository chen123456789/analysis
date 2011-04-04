#ifndef H1DPlot_hh
#define H1DPlot_hh

#include "RootPlot.hh"

#include <QVector>

class TCanvas;
class TH1D;

class H1DPlot : virtual public RootPlot {
public:
  H1DPlot();
  virtual ~H1DPlot();
  static const QVector<DrawOption>& drawOptions();
  virtual void draw(TCanvas*);
  virtual void unzoom();
  void clear();
  virtual void clear(int);
  int numberOfHistograms();
  TH1D* histogram(int i = 0);
  void addHistogram(TH1D*);
private:
  QVector<TH1D*> m_histograms;
  static const QVector<DrawOption> s_drawOptions;
};

#endif
