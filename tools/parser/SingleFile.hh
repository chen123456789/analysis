#ifndef SingleFile_hh
#define SingleFile_hh

#include <QList>
#include <QMap>

class Calibration;
class CalibrationCollection;
class DetectorID;
class PERDaixFiberModule;
class PERDaixTOFModule;
class PERDaixTRDModule;
class PERDaixPMTModule;
class ECALModule;
class QString;
class RawEvent;
class RunFile;
class SimpleEvent;

class SingleFile
{

public:
  SingleFile();
  SingleFile(QString fileName);
  ~SingleFile();

public:
  unsigned int    getNumberOfEvents() const;
  unsigned int    getNumberOfPedestalEvents() const;
  unsigned int    getNumberOfLedEvents() const;
  const RawEvent* getNextRawEvent() const;
  Calibration*    getCalibrationForDetector(DetectorID* id, int whichCali) const;
  unsigned int    getStartTime() const;
  unsigned int    getStopTime() const;

  const CalibrationCollection* calibrate();

private:
  void init();
  void open(QString fileName);
  void addPedestalEvent(CalibrationCollection*, const RawEvent*);
  void addLedEvent(CalibrationCollection*, const RawEvent*);

private:
  RunFile*                      m_runFile;

  QList<PERDaixFiberModule*>    m_fiberModules;
  QList<PERDaixTRDModule*>      m_trdModules;
  QList<PERDaixTOFModule*>      m_tofModules;
  QList<PERDaixPMTModule*>      m_pmtModules;
  QList<ECALModule*>            m_ecalModules;
};

#endif /* SingleFile_hh */
