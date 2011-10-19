#include "TrackerMomentumLikelihood.hh"
#include "Helpers.hh"
#include "Constants.hh"
#include "Particle.hh"

#include <TMath.h>

#include <QDebug>

TrackerMomentumLikelihood::TrackerMomentumLikelihood()
  : Likelihood()
{
  m_likelihoodVariableType = Enums::TrackerMomentumLikelihood;
  m_measuredValueType = Enums::Curvature;
  m_particles = Enums::Proton | Enums::AntiProton | Enums::PiPlus | Enums::PiMinus
    | Enums::Electron | Enums::Positron | Enums::Muon | Enums::AntiMuon;
}

TrackerMomentumLikelihood::~TrackerMomentumLikelihood()
{
}

double TrackerMomentumLikelihood::min() const
{
  return -50.;
}

double TrackerMomentumLikelihood::max() const
{
  return +50.;
}

int TrackerMomentumLikelihood::numberOfParameters() const
{
  return 0;
}

double TrackerMomentumLikelihood::eval(Particle* particle, const KineticVariable& hypothesis, bool* goodInterpolation) const
{
  double curvature = 1. / particle->track()->rigidity(); // measured by tracker only
  return eval(curvature, hypothesis, goodInterpolation);
}

double TrackerMomentumLikelihood::eval(double curvature, const KineticVariable& hypothesis, bool* goodInterpolation) const
{
  if (goodInterpolation)
    *goodInterpolation = true;
  if (curvature < min() || curvature > max())
    return 0;
  double relativeSigma = Helpers::addQuad(0.08 * hypothesis.rigidity(), 0.25 * hypothesis.inverseBeta());
  return TMath::Gaus(curvature, hypothesis.curvature(), relativeSigma, true);
}
