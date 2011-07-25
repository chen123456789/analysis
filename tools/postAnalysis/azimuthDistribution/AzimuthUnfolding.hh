#ifndef AzimuthUnfolding_hh
#define AzimuthUnfolding_hh

#include <QString>

#include <TH2D.h>
#include <TH1D.h>
#include <TGraph.h>

class AzimuthUnfolding {
public:
  AzimuthUnfolding(TH2D* migrationHistogram, TH1D* unfoldInput);
  virtual ~AzimuthUnfolding();
  TH2D* rohIj() {return m_rhoIj;}
  TH1D* unfoldedHistogram() {return m_unfoldedHistogram;}
  TGraph* lCurve() {return m_lCurve;}
  TGraph* bestlcurve() {return m_bestLcurve;}

private:
  TH2D* m_migrationHistogram;
  TH1D* m_unfoldInput;

  void unfold();
  TGraph* m_bestLcurve;
  TH1D* m_unfoldedHistogram;
  TH2D* m_rhoIj;
  TGraph* m_lCurve;

};

#endif
