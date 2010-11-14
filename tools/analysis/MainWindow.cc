#include "MainWindow.hh"

#include "Plotter.hh"
#include "Setup.hh"
#include "Layer.hh"
#include "AnalysisPlot.hh"
#include "CenteredBrokenLine.hh"
#include "BrokenLine.hh"
#include "StraightLine.hh"

#include "BendingPositionPlot.hh"
#include "ResidualPlot.hh"
#include "GeometricOccupancyPlot.hh"

#include <QDebug>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QTimer>

MainWindow::MainWindow(QWidget* parent)
  : QDialog(parent)
  , m_updateTimer(0)
  , m_plotter(0)
  , m_activePlots()
{
  m_ui.setupUi(this);
  
  m_plotter = new Plotter(m_ui.rightWidget);
  m_plotter->setTitleLabel(m_ui.titleLabel);
  m_plotter->setPositionLabel(m_ui.positionLabel);
  m_plotter->setProgressBar(m_ui.progressBar);
  
  QVBoxLayout* layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_plotter);
  m_ui.rightWidget->setLayout(layout);

  m_updateTimer = new QTimer(this);
  connect(m_updateTimer, SIGNAL(timeout()), m_plotter, SLOT(update()));
  
  connect(this, SIGNAL(finished(int)), this, SLOT(mainWindowFinished()));
  connect(m_ui.analyzeButton, SIGNAL(clicked()), this, SLOT(analyzeButtonClicked()));
  connect(m_ui.setFileListButton, SIGNAL(clicked()), this, SLOT(setOrAddFileListButtonClicked()));
  connect(m_ui.addFileListButton, SIGNAL(clicked()), this, SLOT(setOrAddFileListButtonClicked()));
  connect(m_ui.listWidget, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(listWidgetItemChanged(QListWidgetItem*)));
  connect(m_ui.listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(listWidgetCurrentRowChanged(int)));

  connect(m_ui.signalHeightUpperTrackerButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.signalHeightLowerTrackerButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.signalHeightTRDButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.timeOverThresholdButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.trackingButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.occupancyButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.residualsUpperTrackerButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.residualsLowerTrackerButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.residualsTRDButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.momentumReconstructionButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.miscellaneousTrackerButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.miscellaneousTRDButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.miscellaneousTOFButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));

  setupAnalysis();
}

MainWindow::~MainWindow()
{
}

void MainWindow::showButtonsClicked()
{
  QPushButton* b = static_cast<QPushButton*>(sender());
  AnalysisPlot::Topic topic = AnalysisPlot::TopicEnd;
  if (b == m_ui.signalHeightUpperTrackerButton) {
    topic = AnalysisPlot::SignalHeightUpperTracker;
  } else if (b == m_ui.signalHeightLowerTrackerButton) {
    topic = AnalysisPlot::SignalHeightLowerTracker;
  } else if (b == m_ui.signalHeightTRDButton) {
    topic = AnalysisPlot::SignalHeightTRD;
  } else if (b == m_ui.timeOverThresholdButton) {
    topic = AnalysisPlot::TimeOverThreshold;
  } else if (b == m_ui.trackingButton) {
    topic = AnalysisPlot::Tracking;
  } else if (b == m_ui.occupancyButton) {
    topic = AnalysisPlot::Occupancy;
  } else if (b == m_ui.residualsUpperTrackerButton) {
    topic = AnalysisPlot::ResidualsUpperTracker;
  } else if (b == m_ui.residualsLowerTrackerButton) {
    topic = AnalysisPlot::ResidualsLowerTracker;
  } else if (b == m_ui.residualsTRDButton) {
    topic = AnalysisPlot::ResidualsTRD;
  } else if (b == m_ui.momentumReconstructionButton) {
    topic = AnalysisPlot::MomentumReconstruction;
  } else if (b == m_ui.miscellaneousTrackerButton) {
    topic = AnalysisPlot::MiscellaneousTracker;
  } else if (b == m_ui.miscellaneousTRDButton) {
    topic = AnalysisPlot::MiscellaneousTRD;
  } else if (b == m_ui.miscellaneousTOFButton) {
    topic = AnalysisPlot::MiscellaneousTOF;
  }
  if (b->text() == "+") {
    b->setText("-");
    QVector<unsigned int> plotIndices = m_plotter->plotIndices(topic);
    for (int i = 0; i < plotIndices.size(); ++i) {
      m_activePlots.append(plotIndices[i]);
      QListWidgetItem* item = new QListWidgetItem(m_plotter->plotTitle(plotIndices[i]));
      item->setCheckState(Qt::Checked);
      item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
      m_ui.listWidget->addItem(item);
    }
  } else {
    b->setText("+");
    for (int i = m_ui.listWidget->count() - 1; i >= 0; --i) {
      if (m_plotter->plotTopic(m_activePlots[i]) == topic)
        removeListWidgetItem(i);
    }
  }
}

void MainWindow::removeListWidgetItem(int i)
{
  delete m_ui.listWidget->takeItem(i);
  m_activePlots.remove(i);
}

void MainWindow::listWidgetItemChanged(QListWidgetItem* item)
{
  if (item && item->checkState() == Qt::Unchecked) {
    removeListWidgetItem(m_ui.listWidget->row(item));
  }
}

void MainWindow::listWidgetCurrentRowChanged(int i)
{
  if (i < 0 || m_activePlots.size() == 0) {
    m_plotter->selectPlot(-1);
    return;
  }
  m_plotter->selectPlot(m_activePlots[i]);
}

void MainWindow::setupAnalysis()
{
  m_plotter->clearPlots();
  m_activePlots.clear();
  m_ui.listWidget->clear();
  if (m_ui.signalHeightUpperTrackerCheckBox->isChecked()) {
  }
  if (m_ui.signalHeightLowerTrackerCheckBox->isChecked()) {
  }
  if (m_ui.signalHeightTRDCheckBox->isChecked()) {
  }
  if (m_ui.timeOverThresholdCheckBox->isChecked()) {
  }
  if (m_ui.trackingCheckBox->isChecked()) {
    m_plotter->addPlot(new BendingPositionPlot);
  }
  if (m_ui.occupancyCheckBox->isChecked()) {
    Setup* setup = Setup::instance();
    Layer* layer = setup->firstLayer();
    while(layer) {
      m_plotter->addPlot(new GeometricOccupancyPlot(layer->z()));
      layer = setup->nextLayer();
    }
  }
  if (m_ui.residualsUpperTrackerCheckBox->isChecked()) {
    Setup* setup = Setup::instance();
    Layer* layer = setup->firstLayer();
    while(layer) {
      if (layer->z() > 0 && layer->z() < 240)
        m_plotter->addPlot(new ResidualPlot(AnalysisPlot::ResidualsUpperTracker, layer));
      layer = setup->nextLayer();
    }
  }
  if (m_ui.residualsLowerTrackerCheckBox->isChecked()) {
  }
  if (m_ui.residualsTRDCheckBox->isChecked()) {
  }
  if (m_ui.momentumReconstructionCheckBox->isChecked()) {
  }
  if (m_ui.miscellaneousTrackerCheckBox->isChecked()) {
  }
  if (m_ui.miscellaneousTRDCheckBox->isChecked()) {
  }
  if (m_ui.miscellaneousTOFCheckBox->isChecked()) {
  }
  
  m_ui.signalHeightUpperTrackerButton->setText("+");
  m_ui.signalHeightLowerTrackerButton->setText("+");
  m_ui.signalHeightTRDButton->setText("+");
  m_ui.timeOverThresholdButton->setText("+");
  m_ui.trackingButton->setText("+");
  m_ui.occupancyButton->setText("+");
  m_ui.residualsUpperTrackerButton->setText("+");
  m_ui.residualsLowerTrackerButton->setText("+");
  m_ui.residualsTRDButton->setText("+");
  m_ui.momentumReconstructionButton->setText("+");
  m_ui.miscellaneousTrackerButton->setText("+");
  m_ui.miscellaneousTRDButton->setText("+");
  m_ui.miscellaneousTOFButton->setText("+");

  m_ui.signalHeightUpperTrackerButton->setEnabled(m_ui.signalHeightUpperTrackerCheckBox->isChecked());
  m_ui.signalHeightLowerTrackerButton->setEnabled(m_ui.signalHeightLowerTrackerCheckBox->isChecked());
  m_ui.signalHeightTRDButton->setEnabled(m_ui.signalHeightTRDCheckBox->isChecked());
  m_ui.timeOverThresholdButton->setEnabled(m_ui.timeOverThresholdCheckBox->isChecked());
  m_ui.trackingButton->setEnabled(m_ui.trackingCheckBox->isChecked());
  m_ui.occupancyButton->setEnabled(m_ui.occupancyCheckBox->isChecked());
  m_ui.residualsUpperTrackerButton->setEnabled(m_ui.residualsUpperTrackerCheckBox->isChecked());
  m_ui.residualsLowerTrackerButton->setEnabled(m_ui.residualsLowerTrackerCheckBox->isChecked());
  m_ui.residualsTRDButton->setEnabled(m_ui.residualsTRDCheckBox->isChecked());
  m_ui.momentumReconstructionButton->setEnabled(m_ui.momentumReconstructionCheckBox->isChecked());
  m_ui.miscellaneousTrackerButton->setEnabled(m_ui.miscellaneousTrackerCheckBox->isChecked());
  m_ui.miscellaneousTRDButton->setEnabled(m_ui.miscellaneousTRDCheckBox->isChecked());
  m_ui.miscellaneousTOFButton->setEnabled(m_ui.miscellaneousTOFCheckBox->isChecked());

  if (m_ui.trackComboBox->currentText() == "centered broken line") {
    m_plotter->setTrackType(new CenteredBrokenLine());
  } else if (m_ui.trackComboBox->currentText() == "broken line") {
    m_plotter->setTrackType(new BrokenLine());
  } else if (m_ui.trackComboBox->currentText() == "straight line") {
    m_plotter->setTrackType(new StraightLine());
  } else if (m_ui.trackComboBox->currentText() == "none") {
    m_plotter->setTrackType(0);
  }

  m_updateTimer->start(500);
}

void MainWindow::analyzeButtonClicked()
{
  if (m_ui.analyzeButton->text() == "start analysis") {
    m_ui.analyzeButton->setText("abort analysis");
    m_ui.trackComboBox->setEnabled(false);
    setupAnalysis();
    m_plotter->startAnalysis();
  } else {
    m_plotter->abortAnalysis();
    m_ui.trackComboBox->setEnabled(true);
    m_ui.analyzeButton->setText("start analysis");
  }
}

void MainWindow::setOrAddFileListButtonClicked()
{
  QStringList files = QFileDialog::getOpenFileNames(this,
    "Select one or more file lists to open", "", "*.txt;;*.*;;*");
  if (sender() == m_ui.setFileListButton) {
    foreach(QString file, files)
      setFileList(file);
  } else if (sender() == m_ui.addFileListButton) {
    foreach(QString file, files)
      addFileList(file);
  }
}

void MainWindow::addFileList(const QString& fileName)
{
  m_plotter->addFileList(fileName);
}

void MainWindow::setFileList(const QString& fileName)
{
  m_plotter->setFileList(fileName);
}

void MainWindow::saveButtonClicked()
{
  QString fileEnding;
  QString fileName = QFileDialog::getSaveFileName(this, "save event", "", "svg;;pdf;;root;;png", &fileEnding);
  if (fileName.isEmpty())
    return;
  fileEnding.prepend('.');
  if (!fileName.endsWith(fileEnding))
    fileName.append(fileEnding);
  m_plotter->saveCanvas(fileName);
}

void MainWindow::mainWindowFinished()
{
  m_plotter->abortAnalysis();
}