#include "MeasurementTimeDistributionPlot.hh"
#include "SimpleEvent.hh"
#include "Helpers.hh"

#include <TH1D.h>
#include <TLatex.h>
#include <TF1.h>

#include <QDebug>

#include <cmath>

MeasurementTimeDistributionPlot::MeasurementTimeDistributionPlot(int numberOfThreads)
  : AnalysisPlot(AnalysisPlot::MiscellaneousTOF)
  , H1DPlot()
  , m_active(numberOfThreads == 1)
  , m_lastEventTime(-1)
  , m_histogram(0)
{
  setTitle("measurement time distribution plot");
  const int nBins = 5000;
  const double min = 0.05;
  const double max = 600.;
  QVector<double> binning = Helpers::logBinning(nBins, min, max);
  m_histogram = new TH1D(qPrintable("dt " + title()), "", nBins, binning.constData());
  TH1D* histogram = new TH1D(qPrintable(title()), "", nBins, binning.constData());
  setAxisTitle("t_{cut} / s", "measurement time / s");
  addHistogram(histogram);
  if (!m_active) { 
    const int prevNumberOfLatexs = numberOfLatexs();
    addLatex(RootPlot::newLatex(.2, .55));
    latex(prevNumberOfLatexs)->SetTextColor(kRed);
    latex(prevNumberOfLatexs)->SetTitle("This plot has to be filled by only one thread.");
  }
  histogram->GetYaxis()->SetTitleOffset(1.4);
}

MeasurementTimeDistributionPlot::~MeasurementTimeDistributionPlot()
{
}

void MeasurementTimeDistributionPlot::processEvent(const QVector<Hit*>&, const Particle* const, const SimpleEvent* const event)
{
  if (!m_active)
    return;
  double eventTime = event->time();
  if (m_lastEventTime > -1) {
    double deltaT = eventTime - m_lastEventTime;
    //qDebug() << QString::number(eventTime, 30, 10) << QString::number(eventTime2, 30, 10) << QString::number(deltaT, 30, 10);
    if (deltaT < 0)
      qDebug("time between events is smaller than 0, check run list order!");
    m_histogram->Fill(deltaT);
  }
  m_lastEventTime = eventTime;
}

void MeasurementTimeDistributionPlot::update()
{
  double sum = 0;
  for (int cutBin = 1; cutBin <= histogram()->GetNbinsX(); ++cutBin) {
    sum+= m_histogram->GetBinWidth(cutBin) * m_histogram->GetBinContent(cutBin);
    histogram()->SetBinContent(cutBin, sum);
  }
}
