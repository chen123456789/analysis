#include "TotalSignalHeightPlot.hh"

#include <TH1D.h>

#include "TrackInformation.hh"
#include "Track.hh"
#include "Hit.hh"
#include "Cluster.hh"

TotalSignalHeightPlot::TotalSignalHeightPlot() :
  AnalysisPlot(AnalysisPlot::MiscellaneousTracker),
  H1DPlot()
{
  double maximum = 5e4;
  int bins = 1000;
  setTitle(QString("sum of all signal heights"));
  TH1D* histogram = new TH1D(qPrintable(title()), "", bins, 0, maximum);
  histogram->GetXaxis()->SetTitle("signal height / adc counts");
  histogram->GetYaxis()->SetTitle("entries");
  addHistogram(histogram);
}

TotalSignalHeightPlot::~TotalSignalHeightPlot()
{
}

void TotalSignalHeightPlot::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent*)
{
  if(!track)
    return;

  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::AllTrackerLayers))
    return;

  double sum = 0;
  foreach(Hit* hit, hits) {
    if (hit->type() == Hit::tracker) {
      Cluster* cluster = static_cast<Cluster*>(hit);
      sum += cluster->signalHeight();
    }
  }
  histogram()->Fill(sum);
}