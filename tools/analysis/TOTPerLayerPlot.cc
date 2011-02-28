#include "TOTPerLayerPlot.hh"

#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "TOFSipmHit.hh"
#include "TOFCluster.hh"
#include "Track.hh"
#include "Constants.hh"

#include <TH1D.h>
#include <TAxis.h>

#include <QString>
#include <QDebug>

TOTPerLayerPlot::TOTPerLayerPlot(QString layer)
  : AnalysisPlot(TimeOverThreshold)
  , H1DPlot()
  , m_layer(layer)
{
  QString title = QString("time over threshold "+layer+" layer");
  setTitle(title);
  const unsigned int nBinsX = 150;
  const double xMin = 0;
  const double xMax = 300;
  TH1D* histogram = new TH1D(qPrintable(title), "", nBinsX, xMin, xMax);
  histogram->GetXaxis()->SetTitleOffset(1.4);
  histogram->GetXaxis()->SetTitle("time over threshold / ns");
  addHistogram(histogram);
}

TOTPerLayerPlot::~TOTPerLayerPlot()
{}

void TOTPerLayerPlot::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent*) {
  const QVector<Hit*>::const_iterator endIt = hits.end();
  for (QVector<Hit*>::const_iterator it = hits.begin(); it != endIt; ++it) {
    Hit* hit = *it;
    if (hit->type() == Hit::tof) {
      TOFCluster* cluster = static_cast<TOFCluster*> (hit);
      if (qAbs(track->x(cluster->position().z()) - cluster->position().x()) > Constants::tofBarWidth)
        continue;
      if ( !checkLayer( cluster->position().z() ))
        continue;
      std::vector<Hit*>& subHits = cluster->hits();
      std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
      double totSum = 0;
      for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
        Hit* tofHit = *it;
        if (tofHit->detId() == 0x8034) {
          continue;
        }
        TOFSipmHit* tofSipmHit = static_cast<TOFSipmHit*>(tofHit);
        totSum += tofSipmHit->timeOverThreshold();
      }
      histogram()->Fill(totSum);
    }
  }
}

bool TOTPerLayerPlot::checkLayer(double z)
{
  if (m_layer == "upper") {
    if (z > 0) {
      return true;
    }
    else {
      return false;
    }      
  }
  else if (m_layer == "lower") {
    if (z < 0) {
      return true;
    }
    else {
      return false;
    }      
  }
  else if (m_layer == "total") {
    return true;
  }
  else {
    qFatal("Wrong argument passed to TOTPerLayer!");
    return false;
  }
}
