#include "DetectorSelectionWidget.hh"

#include "DetectorIDSpinBox.hh"

#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QCheckBox>

DetectorSelectionWidget::DetectorSelectionWidget(const QVector<unsigned short>& moduleIDs, const int& nChannels) :
  m_moduleSpinBox(new DetectorIDSpinBox(moduleIDs)),
  m_channelSpinBox(new QSpinBox),
  m_individualChannelCheckBox(new QCheckBox("show individual channels"))
{
  m_moduleSpinBox = new DetectorIDSpinBox(moduleIDs);

  QHBoxLayout* layout = new QHBoxLayout(this);

  layout->addWidget(new QLabel("Module"));
  layout->addWidget(m_moduleSpinBox);
  layout->addWidget(m_individualChannelCheckBox);
  layout->addWidget(new QLabel("Channel"));
  layout->addWidget(m_channelSpinBox);

  m_channelSpinBox->setMinimum(0);
  m_channelSpinBox->setMaximum(nChannels-1);
  m_channelSpinBox->setEnabled(false);

  connect(m_individualChannelCheckBox, SIGNAL(stateChanged(int)), this, SLOT(setChannelSpinBoxState(int)));
  connect(m_moduleSpinBox, SIGNAL(valueChanged(int)), this, SLOT(selectedPlotChanged()));
  connect(m_channelSpinBox, SIGNAL(valueChanged(int)), this, SLOT(selectedPlotChanged()));
  connect(m_individualChannelCheckBox, SIGNAL(stateChanged(int)), this, SLOT(selectedPlotChanged()));
}

DetectorSelectionWidget::~DetectorSelectionWidget()
{
}

void DetectorSelectionWidget::selectedPlotChanged()
{
  int module = m_moduleSpinBox->value();
  int channel = m_channelSpinBox->value();
  
  int index = module*(m_channelSpinBox->maximum()+2);
  if (m_individualChannelCheckBox->isChecked())
    index += channel + 1;

  emit(selectPlot(index));
}

void DetectorSelectionWidget::setChannelSpinBoxState(int state)
{
  m_channelSpinBox->setEnabled(state);
}