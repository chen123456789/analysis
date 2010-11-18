#include "MomentumSpectrumPlot.hh"

#include <TH1D.h>

#include "Hit.hh"
#include "TrackSelection.hh"
#include "Track.hh"
#include "BrokenLine.hh"
#include "CenteredBrokenLine.hh"

#include <iostream>

MomentumSpectrumPlot::MomentumSpectrumPlot() :
  AnalysisPlot(AnalysisPlot::MomentumReconstruction),
  H1DPlot()
{
  setTitle("spectrum");
  TH1D* histogram = new TH1D(qPrintable(title()), "", 100, -20, 20);
  histogram->GetXaxis()->SetTitle("p / GeV");
  histogram->GetYaxis()->SetTitle("entries");
  histogram->SetLineColor(kBlack);
  addHistogram(histogram);
}

MomentumSpectrumPlot::~MomentumSpectrumPlot()
{
}

void MomentumSpectrumPlot::processEvent(const QVector<Hit*>&, Track* track, TrackSelection* selection, SimpleEvent* /*event*/)
{
  // QMutexLocker locker(&m_mutex);
  if (!track || !selection || !track->fitGood())
    return;

  TrackSelection::Flags flags = selection->flags();
  if (!(flags & TrackSelection::AllTrackerLayers))
    return;

  double pt = track->pt();

  double r = sqrt(track->x(0)*track->x(0) + track->y(0)*track->y(0));
  if (r < 75) {
    histogram(0)->Fill(pt);
  }
}

void MomentumSpectrumPlot::finalize()
{
}
