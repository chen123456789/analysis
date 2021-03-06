#include "TrackFinding.hh"

#include "Constants.hh"
#include "Hit.hh"
#include "TOFCluster.hh"
#include "StraightLine.hh"
#include "CenteredBrokenLine.hh"
#include "BrokenLine.hh"

#include <TH2D.h>
#include <cmath>
#include <iostream>

#include <QDebug>

int TrackFinding::s_histCounter = 0;

TrackFinding::TrackFinding() :
  m_maxSlope(0.5),
  m_maxOffset(100.),
  m_slopeBins(41),
  m_offsetBins(41),
  m_trackerPull(30.),
  m_trdPull(5.),
  m_tofPull(5.)
{
  char title[128];
  sprintf(title, "trackFindingHist_%d", s_histCounter);
  m_trackFindingHist = new TH2D(title, title, m_slopeBins, -m_maxSlope, m_maxSlope, m_offsetBins, -m_maxOffset, m_maxOffset);
  m_trackFindingHist->GetXaxis()->SetTitle("slope");
  m_trackFindingHist->GetYaxis()->SetTitle("offset / mm");
  s_histCounter++;
}

TrackFinding::~TrackFinding()
{
  delete m_trackFindingHist;
}

QVector<Hit*> TrackFinding::findTrack(const QVector<Hit*>& hits)
{
  m_trackFindingHist->Reset();

  int max = 0;
  int maxBin = 0;
  for (QVector<Hit*>::ConstIterator firstHitIt = hits.begin(); firstHitIt != hits.end(); firstHitIt++) {
    for (QVector<Hit*>::ConstIterator secondHitIt = firstHitIt; secondHitIt != hits.end(); secondHitIt++) {
      if ((*firstHitIt)->type() == Hit::tof || (*secondHitIt)->type() == Hit::tof) continue;
      double x1 = (*firstHitIt)->position().x();
      double x2 = (*secondHitIt)->position().x();
      double z1 = (*firstHitIt)->position().z();
      double z2 = (*secondHitIt)->position().z();

      if (fabs(z2 - z1) > 20) {
        double slope = (x2-x1)/(z2-z1);
        double offset = x1 - slope*z1;
        int bin = m_trackFindingHist->Fill(slope,offset,fabs(z2-z1));
        int content = m_trackFindingHist->GetBinContent(bin);
        if (content > max) {
          max = content;
          maxBin = bin;
        }
      }
    }
  }

  int maxX, maxY, maxZ;
  m_trackFindingHist->GetBinXYZ(maxBin, maxX, maxY, maxZ);

  double slopeGuess = 0.;
  double offsetGuess = 0.;
  double sumOfWeights = 0.;
  if (maxX > 1 && maxX < m_slopeBins && maxY > 1 && maxY < m_offsetBins) {
    for (int i = -1; i <= 1; i++) {
      for (int j = -1; j <= 1; j++) {
        double bc = m_trackFindingHist->GetBinContent(maxX+i, maxY+j);
        slopeGuess += bc*m_trackFindingHist->GetXaxis()->GetBinCenter(maxX+i);
        offsetGuess += bc*m_trackFindingHist->GetYaxis()->GetBinCenter(maxY+j);
        sumOfWeights += bc;
      }
    }
    slopeGuess /= sumOfWeights;
    offsetGuess /= sumOfWeights;
  }
  else {
    slopeGuess = m_trackFindingHist->GetXaxis()->GetBinCenter(maxX);
    offsetGuess = m_trackFindingHist->GetYaxis()->GetBinCenter(maxY);
  }

  StraightLine straightLine;
  straightLine.setSlopeX(slopeGuess);
  straightLine.setX0(offsetGuess);

  QVector<Hit*> hitsForFit;
  const QVector<Hit*>::const_iterator hitsEnd = hits.end();
  for (QVector<Hit*>::const_iterator it = hits.begin(); it != hitsEnd; ++it) {
    Hit* hit = *it;
    if ( (hit->type() == Hit::tracker && isInCorridor(&straightLine, hit, 250)) ||
         (hit->type() == Hit::trd && isInCorridor(&straightLine, hit, 10)) )
      hitsForFit.push_back(hit);
  }

  // improve by redetermining hits on track
  QVector<Hit*> hitsOnTrack;

  CenteredBrokenLine cbl;
  if (cbl.fit(hitsForFit)) {
    const QVector<Hit*>::const_iterator hitsEnd = hits.end();
    for (QVector<Hit*>::const_iterator it = hits.begin(); it != hitsEnd; ++it) {
      Hit* hit = *it;
      double maxPull = 0.;
      if (hit->type() == Hit::tracker)
        maxPull = m_trackerPull;
      if (hit->type() == Hit::trd)
        maxPull = m_trdPull;
      if (hit->type() == Hit::tof) {
        TOFCluster* tofCluster = static_cast<TOFCluster*>(*it);
        int nHits = tofCluster->hits().size();
        if (nHits < 3 || tofCluster->signalHeight() / 10. < nHits * Constants::minimalTotPerSipm)
          continue;
        if (qAbs(cbl.x(tofCluster->position().z()) - tofCluster->position().x()) > 1.1 * Constants::tofBarWidth / 2.)
          continue;
        maxPull = m_tofPull;
      }

      if (isInCorridor(&cbl, hit, maxPull))
        hitsOnTrack.push_back(hit);
    }
  }
  else {
    hitsOnTrack = hitsForFit;
  }

  return hitsOnTrack;
}


bool TrackFinding::isInCorridor(const Track* track, Hit* hit, double maxPull)
{
  Q_ASSERT(track);

  double z = hit->position().z();
  double hitX = 0.5*(hit->position().x() + hit->counterPosition().x());
  double hitY = 0.5*(hit->position().y() + hit->counterPosition().y());
  double trackX = track->x(z);
  double trackY = track->y(z);

  double angle = (-1.)*hit->angle();
  double c = cos(angle);
  double s = sin(angle);
  double hitU = c*hitX - s*hitY;
  double trackU = c*trackX - s*trackY;

  double res = hitU - trackU;
  double resolution = hit->resolutionEstimate();

  return (fabs(res/resolution) < maxPull);
}

