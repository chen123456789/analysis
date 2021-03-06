#include "TRDElectronIdentifierCutBased.hh"

#include <QVector>

#include "TRDReconstruction.hh"
#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "Particle.hh"

TRDElectronIdentifierCutBased::TRDElectronIdentifierCutBased()
  : m_layerCut(5)
  , m_energyDepositionCut(5.) // keV
  , m_energyDepositionPerLengthCut(5./4.) // keV/mm
{
}

bool TRDElectronIdentifierCutBased::isElectronish(const QVector<Hit*>&, const Particle* particle, const SimpleEvent*, bool &ok)
{
  const TRDReconstruction* trdReconst = particle->trdReconstruction();
  if (!(trdReconst->flags() & TRDReconstruction::GoodTRDEvent)) {
    ok = false;
    return false;
  }

  //can be reconstructed
  ok = true;

  QList<double> values;
  if (TRDReconstruction::s_calculateLengthInTube) {
    for (int i = 0; i < 8; ++i)
      values << trdReconst->energyDepositionForLayer(i).edepOnTrackPerLength;
  } else {
    for (int i = 0; i < 8; ++i)
      values << trdReconst->energyDepositionForLayer(i).edepOnTrack;
  }

  qSort(values) ;
  if (values.at(m_layerCut-1) > m_energyDepositionPerLengthCut) {
    return false;
  } else {
    return true;
  }
}
