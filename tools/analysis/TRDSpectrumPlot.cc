#include "TRDSpectrumPlot.hh"

#include <TH1D.h>
#include <TLatex.h>

#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "Cluster.hh"
#include "Hit.hh"
#include "SimpleEvent.hh"
#include "Settings.hh"
#include "SettingsManager.hh"
#include "TRDReconstruction.hh"
#include "Corrections.hh"

#include <math.h>
#include <QList>

TRDSpectrumPlot::TRDSpectrumPlot(unsigned short id, TRDSpectrumType spectrumType) :
  AnalysisPlot(AnalysisPlot::SignalHeightTRD),
  H1DPlot(),
  m_id(id),
  m_spectrumType(spectrumType)
{
  QString strType;
  switch(m_spectrumType){
  case TRDSpectrumPlot::completeTRD:
    strType = "complete TRD";
    break;
  case TRDSpectrumPlot::module:
    strType = "Module";
    break;
  case TRDSpectrumPlot::channel:
    strType = "Channel";
    break;
  case TRDSpectrumPlot::layer:
    strType = "TRD layer";
    break;
  }

  if (m_spectrumType == TRDSpectrumPlot::completeTRD)
    setTitle(strType + QString(" spectrum"));
  else if (m_spectrumType == TRDSpectrumPlot::layer)
    setTitle(strType + QString(" spectrum %1").arg(m_id));
  else
    setTitle(strType + QString(" spectrum 0x%1").arg(m_id,0,16));

  int nBins = TRDReconstruction::s_spectrumDefaultBins;
  double lowerBound = 1e-3;
  double upperBound = TRDReconstruction::spectrumUpperLimit();
  double delta = 1./nBins * (log(upperBound)/log(lowerBound) - 1);
  double p[nBins+1];
  for (int i = 0; i < nBins+1; i++) {
    p[i] = pow(lowerBound, delta*i+1);
  }

  TH1D* histogram = new TH1D(qPrintable(title()), "", nBins, p);
  histogram->Sumw2();
  setAxisTitle(TRDReconstruction::xAxisTitle(), "entries");
  addHistogram(histogram, H1DPlot::HIST);
  setDrawOption(H1DPlot::HIST);

  addLatex(RootPlot::newLatex(0.5, 0.8));
  addLatex(RootPlot::newLatex(0.55, 0.7));
}

TRDSpectrumPlot::~TRDSpectrumPlot()
{
}

void TRDSpectrumPlot::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const)
{
  const TRDReconstruction* trdReconst = particle->trdReconstruction();
  if (!(trdReconst->flags() & TRDReconstruction::GoodTRDEvent))
    return;

  QList<double> valuesToFill;
  switch (m_spectrumType) {
  case TRDSpectrumPlot::completeTRD:
    if (TRDReconstruction::s_calculateLengthInTube)
      for (int i = 0; i < 8; ++i)
        valuesToFill << trdReconst->energyDepositionOnTrackPerLengthForLayer(i);
    else
      for (int i = 0; i < 8; ++i)
        valuesToFill << trdReconst->energyDepositionOnTrackForLayer(i);
    break;
  case TRDSpectrumPlot::module:
    if (TRDReconstruction::s_calculateLengthInTube)
      valuesToFill << trdReconst->energyDepositionOnTrackPerLengthForModule(m_id);
    else
      valuesToFill << trdReconst->energyDepositionOnTrackForModule(m_id);
    break;
  case TRDSpectrumPlot::channel:
    if (TRDReconstruction::s_calculateLengthInTube)
      valuesToFill << trdReconst->energyDepositionOnTrackPerLengthForChannel(m_id);
    else
      valuesToFill << trdReconst->energyDepositionOnTrackForModule(m_id);
    break;
  case TRDSpectrumPlot::layer:
    if (TRDReconstruction::s_calculateLengthInTube)
        valuesToFill << trdReconst->energyDepositionOnTrackPerLengthForLayer(m_id);
    else
        valuesToFill << trdReconst->energyDepositionOnTrackForLayer(m_id);
    break;
  }

  for (QList<double>::const_iterator it = valuesToFill.constBegin(); it != valuesToFill.constEnd(); ++it) {
    if (!(*it > 0.))
      continue;
    int iBin = histogram()->FindBin(*it);
    double width = histogram()->GetBinWidth(iBin);
    double weight = 1./width;
    histogram()->Fill(*it, weight);
  }
}

void TRDSpectrumPlot::update()
{
  latex()->SetTitle(qPrintable(QString("entries = %1").arg(histogram()->GetEntries())));
  latex(1)->SetTitle(qPrintable(QString("uflow = %1, oflow = %2").arg(histogram()->GetBinContent(0)).arg(histogram()->GetBinContent(histogram()->GetNbinsX()))));
}
