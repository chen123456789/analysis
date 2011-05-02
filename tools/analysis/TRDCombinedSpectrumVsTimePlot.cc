#include "TRDCombinedSpectrumVsTimePlot.hh"

#include "DetectorSelectionWidget.hh"
#include "TRDSpectrumVsTimePlot.hh"
#include "Setup.hh"
#include "DetectorElement.hh"

TRDCombinedSpectrumVsTimePlot::TRDCombinedSpectrumVsTimePlot(const QDateTime& first, const QDateTime& last) :
  PlotCollection(AnalysisPlot::SignalHeightTRD)
{
  QVector<unsigned short> moduleIDs;

  Setup* setup = Setup::instance();
  const ElementIterator elementStartIt = setup->firstElement();
  const ElementIterator elementEndIt = setup->lastElement();
  for (ElementIterator elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    if (element->type() == DetectorElement::trd){
      moduleIDs.append(element->id());
      addPlot(new TRDSpectrumVsTimePlot(element->id(),TRDSpectrumPlot::module, first, last));
      for(unsigned short tubeNo = 0; tubeNo < 16; tubeNo++) {
        addPlot(new TRDSpectrumVsTimePlot(element->id() | tubeNo,TRDSpectrumPlot::channel, first, last));
      }
    }
  }

  DetectorSelectionWidget* selectionWidget = new DetectorSelectionWidget(moduleIDs, 16);
  m_secondaryWidget = selectionWidget;
  connect(selectionWidget, SIGNAL(selectPlot(int)), this, SLOT(selectPlot(int)));

  setTitle("spectrum vs time - all modules");
}

TRDCombinedSpectrumVsTimePlot::~TRDCombinedSpectrumVsTimePlot()
{
}