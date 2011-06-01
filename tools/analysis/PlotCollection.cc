#include "PlotCollection.hh"

#include "Plotter.hh"

#include "RootQtWidget.hh"

PlotCollection::PlotCollection(AnalysisPlot::Topic topic) :
  QObject(),
  AnalysisPlot(topic),
  m_selectedPlot(0)
{
}

PlotCollection::~PlotCollection()
{
  qDeleteAll(m_plots);
}

void PlotCollection::processEvent(const QVector<Hit*>& hits, Particle* particle, SimpleEvent* event)
{
  foreach(AnalysisPlot* plot, m_plots)
    plot->processEvent(hits, particle, event);
}

void PlotCollection::finalize()
{
  foreach(AnalysisPlot* plot, m_plots)
    plot->finalize();
}

void PlotCollection::update()
{
  foreach(AnalysisPlot* plot, m_plots)
    plot->update();
}

void PlotCollection::unzoom()
{
  if (m_selectedPlot >= 0 && m_selectedPlot < m_plots.size())
    m_plots[m_selectedPlot]->unzoom();
}

void PlotCollection::positionChanged(double x, double y)
{
  if (m_selectedPlot >= 0 && m_selectedPlot < m_plots.size())
    m_plots[m_selectedPlot]->positionChanged(x,y);
}

void PlotCollection::draw(TCanvas* can)
{
  if (m_selectedPlot >= 0 && m_selectedPlot < m_plots.size())
    m_plots[m_selectedPlot]->draw(can);
  RootPlot::draw(can);
  Plotter::rootWidget()->updateCanvas();
}

void PlotCollection::saveForPostAnalysis(TCanvas* can)
{
  int prevSelectedPlot = m_selectedPlot;
  for (m_selectedPlot = 0; m_selectedPlot < m_plots.size(); m_selectedPlot++) {
    draw(can);
    can->SetName(qPrintable(m_plots.at(m_selectedPlot)->title() + " canvas"));
    can->Write();
  }
  selectPlot(prevSelectedPlot);
}

void PlotCollection::selectPlot(int plot)
{
  m_selectedPlot = plot;
  draw(Plotter::rootWidget()->GetCanvas());
}
