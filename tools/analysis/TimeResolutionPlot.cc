#include "TimeResolutionPlot.hh"
#include "BrokenLine.hh"
#include "TrackInformation.hh"
#include "Hit.hh"
#include "Constants.hh"
#include "TOFCluster.hh"
#include "TOFSipmHit.hh"
#include "Setup.hh"
#include "TOFBar.hh"

#include <TH1.h>
#include <TVector3.h>
#include <TLatex.h>
#include <TF1.h>

#include <QDebug>

TimeResolutionPlot::TimeResolutionPlot(unsigned short idTop1, unsigned short idTop2, unsigned short idBottom1, unsigned short idBottom2)
  : AnalysisPlot(AnalysisPlot::MiscellaneousTOF)
  , H1DPlot()
  , m_idTop1(idTop1)
  , m_idTop2(idTop2)
  , m_idBottom1(idBottom1)
  , m_idBottom2(idBottom2)
  , m_y(0)
{
  QString title = QString("time resolution 0x%1 0x%2 0x%3 0x%4")
    .arg(m_idTop1, 0, 16)
    .arg(m_idTop2, 0, 16)
    .arg(m_idBottom1, 0, 16)
    .arg(m_idBottom2, 0, 16);
  setTitle(title);

  TH1D* histogram = 0;
  TF1* function = 0;
  histogram = new TH1D(qPrintable(title + " all histogram"), "", 30, 0, 6);
  histogram->GetXaxis()->SetTitle("#Deltat / ns");
  addHistogram(histogram);
  function = new TF1(qPrintable(title + " all function"), "gaus(0)", 0, 6);
  addFunction(function);
  histogram = new TH1D(qPrintable(title + " histogram"), "", 30, 0, 6);
  histogram->GetXaxis()->SetTitle("#Deltat / ns");
  histogram->SetLineColor(kRed);
  addHistogram(histogram);
  function = new TF1(qPrintable(title + " function"), "gaus(0)", 0, 6);
  function->SetLineColor(kRed);
  addFunction(function);

  addLatex(RootPlot::newLatex(.15, .85));
  addLatex(RootPlot::newLatex(.15, .82));
  addLatex(RootPlot::newLatex(.15, .79));
  addLatex(RootPlot::newLatex(.15, .76));
  addLatex(RootPlot::newLatex(.15, .73));
  TLatex* latex = 0;
  latex = RootPlot::newLatex(.15, .68); latex->SetTextColor(kRed);
  addLatex(latex);
  latex = RootPlot::newLatex(.15, .65);
  latex->SetTextColor(kRed);
  addLatex(latex);
  latex = RootPlot::newLatex(.15, .62);
  latex->SetTextColor(kRed);
  addLatex(latex);
  latex = RootPlot::newLatex(.15, .59);
  latex->SetTextColor(kRed);
  addLatex(latex);
  latex = RootPlot::newLatex(.15, .56);
  latex->SetTextColor(kRed);
  addLatex(latex);
}

TimeResolutionPlot::~TimeResolutionPlot()
{}

void TimeResolutionPlot::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent*)
{
  // QMutexLocker locker(&m_mutex);
  if (!track || !track->fitGood())
    return;
  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::AllTrackerLayers))
    return;
  if (track->p() < 3)
    return;
  if (qAbs(track->y(Constants::upperTofPosition)-m_y) < 50. && qAbs(track->y(Constants::lowerTofPosition)-m_y) < 50.) {
    histogram(0)->Fill(track->timeOfFlight());
    bool idTop1 = false, idTop2 = false, idBottom1 = false, idBottom2 = false;
    foreach (Hit* hit, hits) {
      unsigned short barId = hit->detId() - hit->channel();
      if (!idTop1 && barId == m_idTop1) idTop1 = true;
      if (!idTop2 && barId == m_idTop2) idTop2 = true;
      if (!idBottom1 && barId == m_idBottom1) idBottom1 = true;
      if (!idBottom2 && barId  == m_idBottom2) idBottom2 = true;
    }
    if (idTop1 && idTop2 && idBottom1 && idBottom2)
      histogram(1)->Fill(track->timeOfFlight());
  }
}

void TimeResolutionPlot::update()
{
  latex(0)->SetTitle(qPrintable(QString("n    = %1").arg(histogram(0)->GetEntries())));
  latex(1)->SetTitle(qPrintable(QString("mean = %1 ns").arg(0.01*qRound(100.*histogram(0)->GetMean()))));
  latex(2)->SetTitle(qPrintable(QString("rms  = %1 ns").arg(0.01*qRound(100.*histogram(0)->GetRMS()))));
  latex(5)->SetTitle(qPrintable(QString("n    = %1").arg(histogram(1)->GetEntries())));
  latex(6)->SetTitle(qPrintable(QString("mean = %1 ns").arg(0.01*qRound(100.*histogram(1)->GetMean()))));
  latex(7)->SetTitle(qPrintable(QString("rms  = %1 ns").arg(0.01*qRound(100.*histogram(1)->GetRMS()))));
}

void TimeResolutionPlot::finalize()
{
  function(0)->SetParameters(histogram(0)->GetMaximum(), histogram(0)->GetMean(), histogram(0)->GetRMS());
  histogram(0)->Fit(function(0), "RQN0");
  latex(3)->SetTitle(qPrintable(QString("t    = %1 ns").arg(0.01*qRound(100.*function(0)->GetParameter(1)))));
  latex(4)->SetTitle(qPrintable(QString("#sigma    = %1 ns").arg(0.01*qRound(100.*function(0)->GetParameter(2)))));
  histogram(1)->Scale(histogram(0)->GetMaximum()/histogram(1)->GetMaximum());
  function(1)->SetParameters(histogram(1)->GetMaximum(), histogram(1)->GetMean(), histogram(1)->GetRMS());
  histogram(1)->Fit(function(1), "RQN0");
  latex(8)->SetTitle(qPrintable(QString("t    = %1 ns").arg(0.01*qRound(100.*function(1)->GetParameter(1)))));
  latex(9)->SetTitle(qPrintable(QString("#sigma    = %1 ns").arg(0.01*qRound(100.*function(1)->GetParameter(2)))));
}
