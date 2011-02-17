#include "TrackingEfficiencyVsMomentumPlot.hh"

#include "Track.hh"
#include "TrackInformation.hh"

#include <QMap>

#include <TH2D.h>

TrackingEfficiencyVsMomentumPlot::TrackingEfficiencyVsMomentumPlot() :
  AnalysisPlot(AnalysisPlot::MiscellaneousTracker),
  H2DPlot()
{
  setTitle(QString("Efficiency vs momentum"));
  
  TH2D* histogram = new TH2D(qPrintable(title()), "", 10, 0., 10., 7, 1.5, 8.5);
  histogram->GetXaxis()->SetTitle("rigidity / GV");
  histogram->GetYaxis()->SetTitle("layers with exactly one hit");
  setHistogram(histogram);
}

TrackingEfficiencyVsMomentumPlot::~TrackingEfficiencyVsMomentumPlot()
{
}

void TrackingEfficiencyVsMomentumPlot::processEvent(const QVector<Hit*>&, Track* track, SimpleEvent*)
{
  if (!track || !track->fitGood())
    return;

  const TrackInformation* info = track->information();

  TrackInformation::Flags flags = info->flags();
  if ( !(flags & TrackInformation::InsideMagnet) || !(flags & TrackInformation::Chi2Good) )
    return;

  QMap<double, int> hitsInLayers = info->hitsInLayers();
  unsigned short nLayers = info->numberOfTrackerLayers();

  foreach(int count, hitsInLayers) {
    if (count != 1)
      return;
  }

  double p = track->p();

  histogram()->Fill(p,nLayers);
}

void TrackingEfficiencyVsMomentumPlot::draw(TCanvas* can)
{
  H2DPlot::draw(can);
  histogram()->SetDrawOption("col.z.text");
}

void TrackingEfficiencyVsMomentumPlot::finalize()
{
  int nBinsX = histogram()->GetNbinsX();
  int nBinsY = histogram()->GetNbinsY();
  for (int binX = 1; binX <= nBinsX; binX++) {
    double sum = 0.;
    for (int binY = 1; binY <= nBinsY; binY++) {
      sum += histogram()->GetBinContent(binX, binY);
    }
    for (int binY = 1; binY <= nBinsY; binY++) {
      double bc = histogram()->GetBinContent(binX, binY);
      histogram()->SetBinContent(binX, binY, bc/sum);
    }    
  }
}