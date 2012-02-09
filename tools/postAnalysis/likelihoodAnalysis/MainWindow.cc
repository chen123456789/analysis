#include "MainWindow.hh"

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "LogLikelihoodPlot.hh"
#include "LogLikelihoodProjectionPlot.hh"
#include "EfficiencyPlot.hh"
#include "RejectionPlot.hh"
#include "RejectionEfficiencyPlot.hh"
#include "LikelihoodPDFPlot.hh"
#include "Setup.hh"

#include <TFile.h>
#include <TROOT.h>
#include <TH2.h>

#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
  : PostAnalysisWindow(parent)
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupAnalysis()
{
  TFile file(qPrintable(m_analysisFiles.at(0)));
  QStringList signalList = PostAnalysisCanvas::savedCanvases(&file, QRegExp("log likelihood histogram .* signal canvas"));
  QStringList backgroundList = PostAnalysisCanvas::savedCanvases(&file, QRegExp("log likelihood histogram .* background canvas"));
  Q_ASSERT(signalList.count() == backgroundList.count());
  gROOT->cd();

  Enums::Particles particles = Setup::instance()->proposedParticles();
  addPlot(new LikelihoodPDFPlot(Enums::SignalHeightTrackerLikelihood, particles));
  for (int layer = 0; layer < 8; ++layer)
    addPlot(new LikelihoodPDFPlot(Enums::SignalHeightTrdLikelihood, particles, layer));
  addPlot(new LikelihoodPDFPlot(Enums::TimeOverThresholdLikelihood, particles));
  addPlot(new LikelihoodPDFPlot(Enums::TimeOfFlightLikelihood, particles));
  addPlot(new LikelihoodPDFPlot(Enums::TrackerMomentumLikelihood, particles));

  int n = signalList.count();
  for (int i = 0; i < n; ++i) {
    PostAnalysisCanvas* signalCanvas = addCanvas(&file, qPrintable(signalList[i]));
    PostAnalysisCanvas* backgroundCanvas = addCanvas(&file, qPrintable(backgroundList[i]));
    LogLikelihoodPlot* logLikelihoodPlot = new LogLikelihoodPlot(signalCanvas, backgroundCanvas);
    addPlot(logLikelihoodPlot);
    int nBins = logLikelihoodPlot->numberOfBins();
    LogLikelihoodProjectionPlot* projection = new LogLikelihoodProjectionPlot(logLikelihoodPlot, 1, nBins);
    addPlot(projection);
    addPlot(new EfficiencyPlot(projection));
    addPlot(new RejectionPlot(projection));
    addPlot(new RejectionEfficiencyPlot(projection));
    for (int bin = 1; bin <= nBins; ++bin) {
      projection = new LogLikelihoodProjectionPlot(logLikelihoodPlot, bin);
      addPlot(projection);
    }
  }

  file.Close();
}