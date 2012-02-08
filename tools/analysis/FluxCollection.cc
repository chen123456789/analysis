#include "FluxCollection.hh"

#include "Helpers.hh"
#include "Constants.hh"
#include "RigidityFlux.hh"
#include "RigiditySpectrumRatio.hh"
#include "RigidityParticleSpectrum.hh"
#include "ParticleInformation.hh"
#include "SimpleEvent.hh"
#include "Particle.hh"
#include "Track.hh"
#include "Hypothesis.hh"

#include <TH1D.h>

#include <cmath>
#include <vector>

#include <QLayout>
#include <QComboBox>
#include <QWidget>
#include <QVector>
#include <QDebug>

FluxCollection::FluxCollection(int numberOfThreads)
  : PlotCollection(Enums::MomentumReconstruction)
  , m_particleHistogram(0)
  , m_particleHistogramAlbedo(0)
{
  QWidget* widget = new QWidget;
  QHBoxLayout* layout = new QHBoxLayout(widget);
  layout->setContentsMargins(0, 0, 0, 0);
  QComboBox* comboBox = new QComboBox;
  layout->addWidget(comboBox);
  layout->addStretch();
  secondaryWidget()->layout()->addWidget(widget);
  setTitle("flux collection");

  const int nBinsData = EfficiencyCorrectionSettings::numberOfBins(EfficiencyCorrectionSettings::Unfolded);
  const double minData = 0.1;
  const double maxData = 20;
  QVector<double> axisData = Helpers::logBinning(nBinsData, minData, maxData);
  int axisSize = axisData.size()*2;
  for (int i = 0; i < axisSize; i+=2) {
    double value = axisData.at(i);
    axisData.prepend(-value);
  }

  m_particleHistogram = new TH1D("particle spectrum - nonAlbedo", "", axisData.size()-1, axisData.constData());
  m_particleHistogram->Sumw2();
  m_particleHistogram->GetXaxis()->SetTitle("rigidity / GV");
  m_particleHistogram->GetYaxis()->SetTitle("particles");

  addPlot(new RigidityParticleSpectrum(RigidityParticleSpectrum::NonAlbedo, m_particleHistogram));
  comboBox->addItem("RigidityParticleSpectrum");
  addPlot(new RigidityFlux(Enums::Positive, numberOfThreads, m_particleHistogram));
  comboBox->addItem("RigidityFlux Positive");
  addPlot(new RigidityFlux(Enums::Negative, numberOfThreads, m_particleHistogram));
  comboBox->addItem("RigidityFlux Negative");

  m_particleHistogramAlbedo = new TH1D("particle spectrum - albedo", "", axisData.size()-1, axisData.constData());
  m_particleHistogramAlbedo->Sumw2();
  m_particleHistogramAlbedo->GetXaxis()->SetTitle("rigidity / GV");
  m_particleHistogramAlbedo->GetYaxis()->SetTitle("particles");

  addPlot(new RigidityParticleSpectrum(RigidityParticleSpectrum::Albedo, m_particleHistogramAlbedo));
  comboBox->addItem("RigidityParticleSpectrum Albedo");
  addPlot(new RigidityFlux(Enums::Positive, numberOfThreads, m_particleHistogramAlbedo, true));
  comboBox->addItem("RigidityFlux Positive Albedo");
  addPlot(new RigidityFlux(Enums::Negative, numberOfThreads, m_particleHistogramAlbedo, true));
  comboBox->addItem("RigidityFlux Negative Albedo");
  addPlot(new RigiditySpectrumRatio());
  comboBox->addItem("RigiditySpectrumRatio");

  connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectPlot(int)));
}

FluxCollection::~FluxCollection()
{
}

void FluxCollection::processEvent(const AnalyzedEvent* event)
{
  PlotCollection::processEvent(event);
  const Track* track = event->goodTrack();
  if (!track)
    return;
  if (!event->flagsSet(ParticleInformation::Chi2Good | ParticleInformation::BetaGood | ParticleInformation::InsideMagnet | ParticleInformation::AllTrackerLayers))
    return;

  if (event->flagsSet(ParticleInformation::Albedo)) {
    m_particleHistogramAlbedo->Fill(-event->particle()->hypothesis()->rigidity());
  } else {
    m_particleHistogram->Fill(event->particle()->hypothesis()->rigidity());
  }
}
