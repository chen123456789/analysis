#include "TOTTemperatureCorrelationPlotCollection.hh"

#include "Setup.hh"
#include "DetectorElement.hh"
#include "TOTTemperatureCorrelationPlot.hh"
#include "TOFChannelSelectionWidget.hh"


TOTTemperatureCorrelationPlotCollection::TOTTemperatureCorrelationPlotCollection() :
  PlotCollection(AnalysisPlot::TimeOverThreshold)
{
  QVector<unsigned short> moduleIDs;

  Setup* setup = Setup::instance();
  const ElementIterator elementStartIt = setup->firstElement();
  const ElementIterator elementEndIt = setup->lastElement();
  for (ElementIterator elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    if (element->type() == DetectorElement::tof){
      for(unsigned short tubeNo = 0; tubeNo < 4; tubeNo++) {
        moduleIDs.append(element->id() | tubeNo);
        addPlot(new TOTTemperatureCorrelationPlot(element->id() | tubeNo));
      }
    }
  }

  TOFChannelSelectionWidget* widget = new TOFChannelSelectionWidget(moduleIDs);
  setSecondaryWidget(widget);
  connect(widget, SIGNAL(channelChanged(int)), this, SLOT(selectPlot(int)));

  setTitle("temperature correlation - all channels");
}

TOTTemperatureCorrelationPlotCollection::~TOTTemperatureCorrelationPlotCollection()
{
}

