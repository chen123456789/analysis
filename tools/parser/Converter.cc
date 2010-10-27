#include "Converter.hh"

#include "SingleFile.hh"
#include "RawEvent.h"
#include "Hit.hh"
#include "TOFSipmHit.hh"
#include "SimpleEvent.hh"
#include "DetectorID.h"
#include "TrackerDataBlock.h"
#include "TRDDataBlock.h"
#include "TOFDataBlock.h"

#include "CLHEP/Units/SystemOfUnits.h"

#include <TVector3.h>
#include <QSettings>
#include <QDebug>

Converter::Converter(const SingleFile* file) :
  m_file(file),
  m_settings(0)
{
  QStringList envVariables = QProcess::systemEnvironment();
  QStringList filteredVars = envVariables.filter(QRegExp("^PERDAIXANA_PATH=*"));
  QString path = "";
  if (filteredVars.size() != 0) {
    QString entry = filteredVars.first();
    path = entry.split("=").at(1);
    path += "/tools/parser/";
  }

  qDebug() << "Using geometry file: " + path + "perdaix_coordinates.conf";
  m_settings = new QSettings(path+"perdaix_coordinates.conf", QSettings::IniFormat);
}

Converter::~Converter()
{
  delete m_settings;
}

SimpleEvent* Converter::generateSimpleEvent(unsigned int eventNo)
{
  const RawEvent* event = m_file->getRawEvent(eventNo);

  // construct new simple event
  int eventId = event->GetEventID();
  int time = event->GetTime();
  SimpleEvent* simpleEvent = new SimpleEvent(eventId, time);

  // loop over all present detector IDs
  QList<DetectorID*> detIDs = event->GetIDs();
  foreach(DetectorID* id, detIDs) {

    //get event data from detector, distinguish types of detectors
    DataBlock* dataBlock = event->GetBlock(id);

    // reserve temporary space
    quint16 nValues = 0;
    if (id->IsTOF())
      nValues = ((TOFDataBlock*) dataBlock)->GetRawDataLength();
    else
      nValues = id->GetDataLength();
    qint16 temp[nValues];

    // get tracker and trd data from block
    int nArrays = 8;
    int nTrd = 2;
    int nMax = 0;
    const quint16* values;
    if (id->IsTracker()) {
      nMax = nArrays;
      values = ((TrackerDataBlock*) dataBlock)->GetRawData();
    }
    else if (id->IsTRD()) {
      nMax = nTrd;
      values = ((TRDDataBlock*) dataBlock)->GetRawData();
    }

    //get calibration for non-tof detectors
    for (int i = 0; i < nMax; i++) {
      if (!id->IsTOF()) {
        Calibration* cali = m_file->getCalibrationForDetector(id, i);
        Q_ASSERT(cali);
        cali->GetAmplitudes(values + i*nValues/nMax, temp + i*nValues/nMax);
      }
    }

    // process data
    int amplitude;
    TVector3 position;

    unsigned short detId = id->GetID16();

    for (int i = 0; i < nValues; i++) {
      // process tracker and trd
      if (id->IsTracker() || id->IsTRD()) {
        amplitude = static_cast<int>(temp[i]);

        if (id->IsTracker()) {
          QList<QVariant> liste = m_settings->value("tracker/"+QString::number(id->GetID16() | i,16)).toList();
          TVector3 pos(liste[0].toDouble(), liste[1].toDouble(), liste[2].toDouble());
          liste = m_settings->value("trackerback/"+QString::number(id->GetID16() | i,16)).toList();
          TVector3 counterPos(liste[0].toDouble(), liste[1].toDouble(), liste[2].toDouble());

          simpleEvent->addHit(new Hit(Hit::tracker, detId | i, amplitude, pos, counterPos));
        }

        if (id->IsTRD()) {
          QList<QVariant> liste = m_settings->value("trd/"+QString::number(id->GetID16() | i,16)).toList();
          TVector3 pos(liste[0].toDouble(), liste[1].toDouble(), liste[2].toDouble());
          liste = m_settings->value("trdback/"+QString::number(id->GetID16() | i,16)).toList();
          TVector3 counterPos(liste[0].toDouble(), liste[1].toDouble(), liste[2].toDouble());

          simpleEvent->addHit(new Hit(Hit::trd, detId | i, amplitude, pos, counterPos));
        }
      } // tracker, trd

      // process tof
      else if (id->IsTOF()) {
        const quint32 value = ((TOFDataBlock*) dataBlock)->GetRawData()[i];
				TofHit tofHit(value);
				TOFSipmHit* tofSipmHit = 0;
				// probably not the most perfomant way but it only has to be done once
				for (unsigned int simpleEventIt = 0; simpleEventIt < simpleEvent->hits().size(); ++simpleEventIt) {
					Hit* hit = simpleEvent->hits()[simpleEventIt];
					if (hit->type() != Hit::tof)
						continue;
					tofSipmHit = static_cast<TOFSipmHit*>(hit);
					if (tofSipmHit->channel() == tofHit.channel()) {
						break;
					} else {
						tofSipmHit = 0;
					}
				}
				if (!tofSipmHit) {
      		QList<QVariant> liste = m_settings->value("tof/"+QString::number(id->GetID16() | tofHit.channel(),16)).toList();
      		TVector3 pos(liste[0].toDouble(), liste[1].toDouble(), liste[2].toDouble());
      		liste = m_settings->value("tofback/"+QString::number(id->GetID16() | tofHit.channel(),16)).toList();
      		TVector3 counterPos(liste[0].toDouble(), liste[1].toDouble(), liste[2].toDouble());
					tofSipmHit = new TOFSipmHit(detId | tofHit.channel(), pos, counterPos);
					simpleEvent->addHit(tofSipmHit);
				}
				tofSipmHit->addLevelChange(value);
      } // tof

		} // tracker, trd, tofevent

  } // foreach(DetectorID...)

  return simpleEvent;
}
