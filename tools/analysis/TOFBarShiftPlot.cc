#include "TOFBarShiftPlot.hh"
#include "BrokenLine.hh"
#include "ParticleInformation.hh"
#include "Hit.hh"
#include "Constants.hh"
#include "TOFCluster.hh"
#include "TOFSipmHit.hh"
#include "Setup.hh"
#include "TOFBar.hh"
#include "Particle.hh"
#include "Track.hh"
#include "TimeOfFlight.hh"

#include <TH1.h>
#include <TVector3.h>
#include <TLatex.h>
#include <TF1.h>

#include <QDebug>

TOFBarShiftPlot::TOFBarShiftPlot(unsigned short idTop1, unsigned short idTop2, unsigned short idBottom1, unsigned short idBottom2)
  : AnalysisPlot(AnalysisPlot::CalibrationTOF)
  , H1DPlot()
  , m_idTop1(idTop1)
  , m_idTop2(idTop2)
  , m_idBottom1(idBottom1)
  , m_idBottom2(idBottom2)
{
  QString title = QString("bar shift 0x%1 0x%2 0x%3 0x%4")
    .arg(m_idTop1, 0, 16)
    .arg(m_idTop2, 0, 16)
    .arg(m_idBottom1, 0, 16)
    .arg(m_idBottom2, 0, 16);
  setTitle(title);
  TH1D* histogram = new TH1D(qPrintable(title), "", 30, 0, 6);
  setAxisTitle("#Deltat / ns", "");
  addHistogram(histogram);
  TF1* function = new TF1(qPrintable(title + " function"), "gaus(0)", 0, 6);
  function->SetLineColor(kRed);
  addFunction(function);
  addLatex(RootPlot::newLatex(.15, .85));
  addLatex(RootPlot::newLatex(.15, .82));
  addLatex(RootPlot::newLatex(.15, .79));
  addLatex(RootPlot::newLatex(.15, .76));
  addLatex(RootPlot::newLatex(.15, .73));
}

TOFBarShiftPlot::~TOFBarShiftPlot()
{}

void TOFBarShiftPlot::processEvent(const QVector<Hit*>& hits, Particle* particle, SimpleEvent*)
{
  const Track* track = particle->track();
  const TimeOfFlight* tof = particle->timeOfFlight();

  // QMutexLocker locker(&m_mutex);
  if (!track || !track->fitGood())
    return;
  ParticleInformation::Flags flags = particle->information()->flags();
  if (!(flags & (ParticleInformation::Chi2Good | ParticleInformation::InsideMagnet)))
    return;
  if (track->rigidity() < 1)
    return;
  bool idTop1 = false, idTop2 = false, idBottom1 = false, idBottom2 = false;
  const QVector<Hit*>::const_iterator endIt = hits.end();
  for (QVector<Hit*>::const_iterator it = hits.begin(); it != endIt; ++it) {
    Hit* hit = *it;
    unsigned short barId = hit->detId() - hit->channel();
    if (!idTop1 && barId == m_idTop1) idTop1 = true;
    if (!idTop2 && barId == m_idTop2) idTop2 = true;
    if (!idBottom1 && barId == m_idBottom1) idBottom1 = true;
    if (!idBottom2 && barId  == m_idBottom2) idBottom2 = true;
  }
  if (idTop1 && idTop2 && idBottom1 && idBottom2) {
    double l = track->trackLength();
    double d = Constants::upperTofPosition - Constants::lowerTofPosition;
    double lCorrection = (d - l) / Constants::speedOfLight;
    double m = particle->mass();
    double rigidity = track->rigidity();
    double t = tof->timeOfFlight();
    double pCorrection = (t + lCorrection) * (1 - sqrt(rigidity*rigidity + m*m) / rigidity);
    double yu = track->y(Constants::upperTofPosition);
    double yl = track->y(Constants::lowerTofPosition);
    if (qAbs(yu) < 50. && qAbs(yl) < 50.)
      histogram()->Fill(t + lCorrection + pCorrection);
  }
}

void TOFBarShiftPlot::update()
{
  latex(0)->SetTitle(qPrintable(QString("n    = %1").arg(histogram(0)->GetEntries())));
  latex(1)->SetTitle(qPrintable(QString("mean = %1 ns").arg(histogram(0)->GetMean(), 0, 'f', 2, ' ')));
  latex(2)->SetTitle(qPrintable(QString("rms  = %1 ns").arg(histogram(0)->GetRMS(), 0, 'f', 2, ' ')));
}

void TOFBarShiftPlot::finalize()
{
  function(0)->SetParameters(histogram(0)->GetMaximum(), histogram(0)->GetMean(), histogram(0)->GetRMS());
  histogram(0)->Fit(function(0), "RQN0");
  latex(3)->SetTitle(qPrintable(QString("t    = %1 ns").arg(function(0)->GetParameter(1), 0, 'f', 2, ' ')));
  latex(4)->SetTitle(qPrintable(QString("#sigma    = %1 ns").arg(function(0)->GetParameter(2), 0, 'f', 2, ' ')));
}