#include "MainWindow.hh"

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "ProjectionPlot.hh"

#include <TCanvas.h>
#include <TFile.h>
#include <TROOT.h>

#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
  : PostAnalysisWindow(parent)
  , m_files()
  , m_projectionPlots()
{
}

MainWindow::~MainWindow()
{
  foreach (TFile* file, m_files) {
    file->Close();
    delete file;
  }
}

void MainWindow::setupAnalysis()
{
  typedef QPair<QString, TFile*> Key;
  QVector<Key> allKeys;
  foreach (QString fileName, m_analysisFiles) {
    TFile* file = new TFile(qPrintable(fileName));
    TNamed* analysisSetting = static_cast<TNamed*>(file->Get("analysis setting"));
    if (analysisSetting) {
      qDebug();
      qDebug() << analysisSetting->GetTitle();
    }
    QStringList fileKeyList = PostAnalysisCanvas::savedCanvases(file);
    gROOT->cd();
    foreach (QString key, fileKeyList) {
      if (1 < m_analysisFiles.count())
        key+= " (" + fileName + ")";
      allKeys.append(Key(key, file));
    }
    m_files.append(file);
  }
  qSort(allKeys);
  foreach (Key key, allKeys) {
    PostAnalysisCanvas* canvas = new PostAnalysisCanvas(key.second, key.first);
    QVector<TH2D*> histograms = canvas->histograms2D();
    if (histograms.count() == 1) {
      addCanvas(canvas);
      ProjectionPlot* plot = new ProjectionPlot(histograms.first());
      m_projectionPlots.append(plot);
      addPlot(plot);
    } else {
      delete canvas;
    }
  }
}