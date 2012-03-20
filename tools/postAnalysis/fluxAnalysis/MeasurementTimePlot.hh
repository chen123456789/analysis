#ifndef MeasurementTimePlot_hh
#define MeasurementTimePlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QObject>
#include <QVector>

class PostAnalysisCanvas;
class ParameterWidget;
class TLine;

class MeasurementTimePlot : public QObject, public PostAnalysisPlot, public H1DPlot
{
Q_OBJECT
public:
  MeasurementTimePlot(PostAnalysisCanvas*);
  ~MeasurementTimePlot();
  double measurementTime() const;
  virtual void draw(TCanvas*);
public slots:
  virtual void update();
protected:
  ParameterWidget* m_cutParameterWidget;
  TLine* m_cutLine;
};

#endif