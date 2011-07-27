#include "TRDSpectrumVsTemperaturePlot.hh"

#include "Particle.hh"
#include "ParticleInformation.hh"
#include "Track.hh"
#include "Cluster.hh"
#include "Hit.hh"
#include "SimpleEvent.hh"
#include "TRDReconstruction.hh"
#include "Settings.hh"
#include "SettingsManager.hh"

#include <TH2D.h>
#include <TAxis.h>

#include <QString>
#include <QDebug>
#include <QVariant>
#include <QSettings>
#include <math.h>

TRDSpectrumVsTemperaturePlot::TRDSpectrumVsTemperaturePlot(unsigned short id, TRDSpectrumPlot::TRDSpectrumType spectrumType)
  : AnalysisPlot(AnalysisPlot:: SignalHeightTRD)
  , H2DPlot()
  , m_id(id)
  , m_spectrumType(spectrumType)
{
  QString strType;
  switch(m_spectrumType){
  case TRDSpectrumPlot::completeTRD:
    strType = "complete TRD vs temperature";
    break;
  case TRDSpectrumPlot::module:
    strType = "module vs temperature";
    break;
  case TRDSpectrumPlot::channel:
    strType = "channel vs temperature";
    break;
  case TRDSpectrumPlot::layer:
    strType = "layer vs temperature";
    break;
  }

  if (m_spectrumType == TRDSpectrumPlot::completeTRD)
    setTitle(strType + QString(" spectrum"));
  else
    setTitle(strType + QString(" spectrum 0x%1").arg(m_id,0,16));


  const unsigned int nTemperatureBins = 200;
  const double minTemperature = 26;
  const double maxTemperature = 34;
  int nBins = TRDReconstruction::s_spectrumDefaultBins;
  double lowerBound = 1e-3;
  double upperBound = TRDReconstruction::spectrumUpperLimit();
  double delta = 1./nBins * (log(upperBound)/log(lowerBound) - 1);
  double p[nBins+1];
  for (int i = 0; i < nBins+1; i++) {
    p[i] = pow(lowerBound, delta*i+1);
  }

  TH2D* histogram = new TH2D(qPrintable(title()),"", nTemperatureBins, minTemperature, maxTemperature, nBins, p);
  setAxisTitle("temperature /  #circC", TRDReconstruction::xAxisTitle(), "");
  addHistogram(histogram);
}

TRDSpectrumVsTemperaturePlot::~TRDSpectrumVsTemperaturePlot()
{
}

void TRDSpectrumVsTemperaturePlot::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const event)
{
  const TRDReconstruction* trdReconst = particle->trdReconstruction();
  if (!(trdReconst->flags() & TRDReconstruction::GoodTRDEvent))
    return;

  // TODO: temp sensormap
  double meanTemperature = 0.;
  int count = 0;
  for (unsigned int i = SensorTypes::TRD_TUBE_TOP_HOT_TEMP; i <= SensorTypes::TRD_TUBE_BOTTOM_COLD_TEMP; i++) {
    meanTemperature += event->sensorData((SensorTypes::Type)i);
    count++;
  }
  meanTemperature /= count;

  QVector<double> valuesToFill;
  switch (m_spectrumType) {
  case TRDSpectrumPlot::completeTRD:
    if (TRDReconstruction::s_calculateLengthInTube) {
      for (int i = 0; i < 8; ++i)
        if (trdReconst->energyDepositionForLayer(i).lengthThroughTube > 0.)
          valuesToFill << trdReconst->energyDepositionForLayer(i).edepOnTrackPerLength;
    } else {
      for (int i = 0; i < 8; ++i)
        valuesToFill << trdReconst->energyDepositionForLayer(i).edepOnTrack;
    }
    break;
  case TRDSpectrumPlot::module:
    if (TRDReconstruction::s_calculateLengthInTube) {
      if (trdReconst->energyDepositionForModule(m_id).lengthThroughTube > 0.)
        valuesToFill << trdReconst->energyDepositionForModule(m_id).edepOnTrackPerLength;
    } else {
      valuesToFill << trdReconst->energyDepositionForModule(m_id).edepOnTrack;
    }
    break;
  case TRDSpectrumPlot::channel:
    if (TRDReconstruction::s_calculateLengthInTube) {
      if (trdReconst->energyDepositionForChannel(m_id).lengthThroughTube > 0.)
        valuesToFill << trdReconst->energyDepositionForChannel(m_id).edepOnTrackPerLength;
    } else {
      valuesToFill << trdReconst->energyDepositionForChannel(m_id).edepOnTrack;
    }
    break;
  case TRDSpectrumPlot::layer:
    if (TRDReconstruction::s_calculateLengthInTube) {
      if (trdReconst->energyDepositionForLayer(m_id).lengthThroughTube > 0.)
        valuesToFill << trdReconst->energyDepositionForLayer(m_id).edepOnTrackPerLength;
    } else {
      valuesToFill << trdReconst->energyDepositionForLayer(m_id).edepOnTrack;
    }
    break;
  }

  for (QVector<double>::const_iterator it = valuesToFill.constBegin(); it != valuesToFill.constEnd(); ++it) {
    if (!(*it > 0.))
      continue;
    int iGlobalBin = histogram()->FindBin(meanTemperature, *it);
    int iXBin, iYBin, iZBin;
    histogram()->GetBinXYZ(iGlobalBin, iXBin, iYBin, iZBin);
    double width = histogram()->GetBinWidth(iYBin);
    double weight = 1./width;
    histogram()->Fill(meanTemperature, *it, weight);
  }
}
