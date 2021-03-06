#include "AzimuthPositionCorrelation.hh"

#include <QDebug>

#include <TH2D.h>

#include "Hit.hh"
#include "ParticleInformation.hh"
#include "Particle.hh"
#include "Track.hh"
#include "SimpleEvent.hh"
#include "ProjectionControlWidget.hh"
#include "Constants.hh"

#include <QSpinBox>

#include <iostream>
#include <cmath>

AzimuthPositionCorrelation::AzimuthPositionCorrelation(Direction direction, Enums::ChargeSigns type)
  : AnalysisPlot(Enums::Tracking)
  , H2DProjectionPlot()
  , m_direction(direction)
  , m_type(type)
{
  QString htitle;
  if (direction == X) {
    htitle = "Azimuth correlation with x direction";
  } else {
    htitle = "Azimuth correlation with y direction";
  }

  if (m_type == Enums::Positive)
    htitle += " positive";
  if (m_type == Enums::Negative)
    htitle += " negative";
  if (m_type == (Enums::Positive | Enums::Negative))
    htitle += " all";
  setTitle(htitle);

  const int nBinsX = 45;
  const double xMin = -180;
  const double xMax = 180;

  const int nBinsY = 40;
  const double yMin = -Constants::tofBarLength / 2.;
  const double yMax = Constants::tofBarLength / 2.;

  TH2D* histogram = new TH2D(qPrintable(htitle), "", nBinsX, xMin, xMax, nBinsY, yMin, yMax);
  histogram->Sumw2();
  if (direction == X) {
    setAxisTitle("azimuth/degree", "x position / mm", "");
  } else {
    setAxisTitle("azimuth/degree", "y position / mm", "");
  }
  addHistogram(histogram);

  setDrawOption(COLZ);
  controlWidget()->spinBox()->setMaximum(histogram->GetNbinsY());
}

AzimuthPositionCorrelation::~AzimuthPositionCorrelation()
{
}

void AzimuthPositionCorrelation::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const)
{
  const Track* track = particle->track();

  if (!track || !track->fitGood())
    return;

  ParticleInformation::Flags flags = particle->information()->flags();
  if ( !(flags & ParticleInformation::Chi2Good) || !(flags & ParticleInformation::InsideMagnet)
    || !(flags & ParticleInformation::BetaGood) || (flags & ParticleInformation::Albedo) )
    return;

  double rigidity = track->rigidity();
  if (m_type == Enums::Positive && rigidity < 0)
    return;
  if (m_type == Enums::Negative && rigidity > 0)
    return;

  double azimuth = track->azimuthAngle() * 180. / M_PI;
  double position = 0;
  if (m_direction == X) {
    position = track->x(Constants::upperTofPosition);
  } else {
    position = track->y(Constants::upperTofPosition);
  }

  histogram()->Fill(azimuth, position);
}
