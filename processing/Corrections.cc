#include "Corrections.hh"

#include "Hit.hh"
#include "TOFSipmHit.hh"
#include "TOFCluster.hh"
#include "Setup.hh"
#include "DetectorElement.hh"
#include "TOFBar.hh"

#include <QProcess>
#include <QSettings>

#include <cstring>

Corrections::Corrections(Flags flags) :
  m_trdSettings(0),
  m_flags(flags)
{
  QStringList envVariables = QProcess::systemEnvironment();
  QStringList filteredVars = envVariables.filter(QRegExp("^PERDAIXANA_PATH=*"));
  QString path = "";
  if (filteredVars.size() != 0) {
    QString entry = filteredVars.first();
    path = entry.split("=").at(1);
    path += "/conf/";
  }
  else {
    qFatal("ERROR: You need to set PERDAIXANA_PATH environment variable to the toplevel location!");
  }
  m_trdSettings = new QSettings(path+"TRDCorrections.conf", QSettings::IniFormat);
}

Corrections::~Corrections()
{
  delete m_trdSettings;
}

void Corrections::apply(QVector<Hit*>& hits)
{
  foreach(Hit* hit, hits) {
    if (m_flags & Alignment) alignment(hit);
    if (m_flags & TimeShifts) timeShift(hit);
    if (m_flags & TrdMopv) trdMopv(hit);
    if (m_flags & TofTimeOverThreshold) tofTimeOverThreshold(hit);
  }
}

void Corrections::alignment(Hit* hit)
{
  Setup* setup = Setup::instance();
  DetectorElement* element = setup->element(hit->detId() - hit->channel());
  if (element->alignmentShift() != 0.)
    hit->setPosition(element->positionForHit(hit));
}

void Corrections::timeShift(Hit* hit)
{
  Setup* setup = Setup::instance();
  if (strcmp(hit->ClassName(), "TOFSipmHit") == 0) {
    TOFSipmHit* tofHit = static_cast<TOFSipmHit*>(hit);
    DetectorElement* element = setup->element(hit->detId() - hit->channel());
    double timeShift = static_cast<TOFBar*>(element)->timeShift(hit->channel());
    tofHit->applyTimeShift(timeShift);
  }
  else if (strcmp(hit->ClassName(), "TOFCluster") == 0) {
    TOFCluster* cluster = static_cast<TOFCluster*>(hit);
    std::vector<Hit*> subHits = cluster->hits();
    for (std::vector<Hit*>::iterator it = subHits.begin(); it != subHits.end(); it++) {
      TOFSipmHit* tofHit = static_cast<TOFSipmHit*>(*it);
      DetectorElement* element = setup->element(cluster->detId() - cluster->channel());
      double timeShift = static_cast<TOFBar*>(element)->timeShift((*it)->channel());
      tofHit->applyTimeShift(timeShift);
    }
    cluster->processHits();
  }
}

void Corrections::trdMopv(Hit* hit)
{
  //only process TRD hits
  if ( hit->type() != Hit::trd )
    return;

  if (strcmp(hit->ClassName(), "Hit") == 0) {
    double trdScalingFactor = this->trdScalingFactor(hit->detId()) ;
    hit->setSignalHeight(hit->signalHeight() * trdScalingFactor) ;
  }
  else if (strcmp(hit->ClassName(), "Cluster") == 0) {
    Cluster* cluster = static_cast<Cluster*>(hit) ;
    double clusterAmplitude = 0;
    for (std::vector<Hit*>::iterator it = cluster->hits().begin(); it != cluster->hits().end(); it++) {
      Hit* trdHit = *it;
      double trdScalingFactor = this->trdScalingFactor(trdHit->detId()) ;
      double newHitAmplitude = trdHit->signalHeight() * trdScalingFactor ;
      trdHit->setSignalHeight(newHitAmplitude) ;
      clusterAmplitude += newHitAmplitude ;
    }
    cluster->setSignalHeight(clusterAmplitude);
  }
}

void Corrections::tofTimeOverThreshold(Hit*)
{
}

double Corrections::trdScalingFactor(unsigned int channel)
{
  return m_trdSettings->value( "ConstScaleFactor/" + QString::number(channel,16) , 1.).toDouble() ;
}

void Corrections::setTrdScalingFactor(unsigned int channel, double value)
{
  m_trdSettings->setValue( "ConstScaleFactor/" + QString::number(channel,16), value) ;
  m_trdSettings->sync();
}

