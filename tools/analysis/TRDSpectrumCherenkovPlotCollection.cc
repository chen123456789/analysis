#include "TRDSpectrumCherenkovPlotCollection.hh"

#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>

#include "StringSpinBox.hh"
#include "TRDSpectrumCherenkovPlot.hh"


TRDSpectrumCherenkovPlotCollection::TRDSpectrumCherenkovPlotCollection() :
  PlotCollection(Enums::Testbeam)
{
  QComboBox* comboBox = new QComboBox();

  comboBox->addItem("no cherenkov cut");
  addPlot(new TRDSpectrumCherenkovPlot(TRDSpectrumCherenkovPlot::None));
  comboBox->addItem("below both");
  addPlot(new TRDSpectrumCherenkovPlot(TRDSpectrumCherenkovPlot::BothBelow));
  comboBox->addItem("above both");
  addPlot(new TRDSpectrumCherenkovPlot(TRDSpectrumCherenkovPlot::BothAbove));
  comboBox->addItem("c1 above and c2 below");
  addPlot(new TRDSpectrumCherenkovPlot(TRDSpectrumCherenkovPlot::C1AboveC2Below));
  comboBox->addItem("c1 below and c2 above");
  addPlot(new TRDSpectrumCherenkovPlot(TRDSpectrumCherenkovPlot::C1BelowC2Above));

  QWidget* selectionWidget = new QWidget();

  QHBoxLayout* layout = new QHBoxLayout(selectionWidget);
  layout->addStretch();
  layout->addWidget(new QLabel("cherenkov cut settings"));
  layout->addWidget(comboBox);
  layout->addStretch();

  secondaryWidget()->layout()->addWidget(selectionWidget);
  connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectPlot(int)));

  setTitle("TRD spectra - cherenkov cuts");
}

TRDSpectrumCherenkovPlotCollection::~TRDSpectrumCherenkovPlotCollection()
{
}
