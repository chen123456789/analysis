#include "MCTRDCalibrationPlot.hh"

#include <QDebug>

#include <TH2D.h>

#include "SimpleEvent.hh"
#include "MCSimpleEventDigi.hh"
#include "MCDigiSignal.hh"
#include "Cluster.hh"

MCTRDCalibrationPlot::MCTRDCalibrationPlot()
  : AnalysisPlot(AnalysisPlot::MonteCarloTRD)
  , H2DPlot()
{
  setTitle("MC TRD Calibration");
  TH2D* histogram = new TH2D(qPrintable(title()), "", 100, 0, 100, 100, -100, 3000);
  setAxisTitle("MC energy deposition / keV", "MC Signal ADCCs", "");
  addHistogram(histogram);
}

void MCTRDCalibrationPlot::processEvent(const QVector<Hit*>&, Particle*, SimpleEvent* event)
{
  qDebug("new event");
  if (event->contentType() != SimpleEvent::MonteCarlo)
    return;

  const MCEventInformation* mcInfo = event->MCInformation();

  if(!mcInfo)
    return;


  const std::vector<const MCSimpleEventDigi*> mcDigis = mcInfo->mcDigis();
  const std::vector<Hit*> hits = event->hits();
  QVector<Hit*> trdHits;

  //get all trd channel hits:
  std::vector<Hit*>::const_iterator itHit;
  for (itHit = hits.begin(); itHit != hits.end(); ++itHit) {
    Hit* hit = *itHit;
    if (hit->type() != Hit::trd)
      continue;
    Cluster* cluster = static_cast<Cluster*>(hit);
    std::vector<Hit*>& subHits = cluster->hits();
    const std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
    for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
      trdHits << *it;
      qDebug("trd hit id = 0x%x at pos (%f %f %f)", (*it)->detId(), (*it)->position().x(), (*it)->position().y(), (*it)->position().z());
    }
  }

  qDebug("trdhits: %i", trdHits.size());

  std::vector<const MCSimpleEventDigi*>::const_iterator itDigi;
  for (itDigi = mcDigis.begin(); itDigi != mcDigis.end(); ++itDigi) {
    if ((*itDigi)->Type() != Hit::trd)
      continue;
    double enDep = (*itDigi)->SignalSum();
    unsigned short channelID = (*itDigi)->DetID();

    //bugfix for atm wrong digiIds:
    //qDebug("channelID 0x%x", channelID);
    unsigned short bitMaskWrong = 3 << 5; // 0b01100000
    unsigned short bitMaskRight = 3 << 4; // 0b00110000
    unsigned short moduleIDShifted = (channelID & bitMaskWrong);
    //qDebug("wrong module 0x%x", moduleIDShifted);
    moduleIDShifted =  moduleIDShifted >> 1;
    //qDebug("module 0x%x", moduleIDShifted);
    channelID = channelID & ~bitMaskWrong;
    //qDebug("deleted module 0x%x", channelID);
    channelID = channelID | moduleIDShifted;
    //correct for wrong channel:
    unsigned int channel = channelID & 0xF;
    if (channel == 7)
      continue;
    if (channel > 7)
      channelID += 1;
    //end bugfix


    TVector3 pos = (*itDigi)->Signals().at(0)->hitPosition;
    qDebug("trd digi id = 0x%x at pos (%f %f %f)", channelID, pos.x(), pos.y(), pos.z());

    //find corresponding hit
    double signal = 0.;
    QVector<Hit*>::const_iterator itHit;
    for (itHit = trdHits.begin(); itHit != trdHits.end(); ++itHit) {
      if (channelID == (*itHit)->detId()) {
        signal = (*itHit)->signalHeight();
        break;
      }
    }

    qDebug("Filling %f, %f", enDep, signal);
    histogram()->Fill(enDep, signal);
  }

}
