#include "BendingPositionPlot.hh"
#include "BrokenLine.hh"

#include "Hit.hh"

#include <TH1.h>

BendingPositionPlot::BendingPositionPlot()
  : H1DPlot(AnalysisPlot::Tracking, "bending position", 1000, -1000, 1000)
{
  m_histogram->GetXaxis()->SetTitle("z / mm");
}

BendingPositionPlot::~BendingPositionPlot()
{}

void BendingPositionPlot::processEvent(const QVector<Hit*>& clusters, Track* track, SimpleEvent*)
{
  if (track->type() == Track::BrokenLine) {
    BrokenLine* line = static_cast<BrokenLine*>(track);
    int nTrackerHits = 0;
    foreach(Hit* hit, clusters)
      if (hit->type() == Hit::tracker)
        ++nTrackerHits;
    if (nTrackerHits != 8)
      return;
    m_histogram->Fill(line->zIntersection());
  }
}

void BendingPositionPlot::finalize()
{
}