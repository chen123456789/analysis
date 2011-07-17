#ifndef MultiLayerEfficiencyCorrection_hh
#define MultiLayerEfficiencyCorrection_hh

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QString>
#include <QVector>
#include <QMap>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TH1D.h>

class MultiLayerEfficiencyCorrection : public PostAnalysisPlot, public H1DPlot {
public:
  MultiLayerEfficiencyCorrection(PostAnalysisCanvas*);
  virtual ~MultiLayerEfficiencyCorrection();
  
public:
  
private:
  QString m_name;
  
  void saveAsSetting();

};

#endif
