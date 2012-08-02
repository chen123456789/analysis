#ifndef MainWindow_h
#define MainWindow_h

#include "PostAnalysisWindow.hh"

class PostAnalysisPlot;
class TFile;
class ProjectionPlot;

class MainWindow : public PostAnalysisWindow {
Q_OBJECT
public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();
protected:
  virtual void setupAnalysis();
  QVector<TFile*> m_files;
  QVector<ProjectionPlot*> m_projectionPlots;
};

#endif