#ifndef Hit_hh
#define Hit_hh

#include <TObject.h>
#include <TVector3.h>

class Hit :
  public TObject
{

public:
  enum ModuleType {none=-1, tracker, trd, tof};

public:
  Hit();

  Hit(ModuleType typ,
      unsigned short detId,
      double signalHeight,
      TVector3 position,
      TVector3 counterPosition);

  Hit(const Hit& other);

  virtual ~Hit();

public:
  void setSignalHeight(const double height) {m_signalHeight = height;}
  void setPosition(const TVector3& pos) {m_position = pos;}
  void setCounterPosition(const TVector3& pos) {m_counterPosition = pos;}
  void setResolution(const double& res) {m_resolution = res;}

public:
  ModuleType      type()            const {return m_type;}
  unsigned short  detId()           const {return m_detId;}
  double          signalHeight()    const {return m_signalHeight;}
  const TVector3& position()        const {return m_position;}
  const TVector3& counterPosition() const {return m_counterPosition;}
  double          angle()           const {return m_angle;}

  unsigned short  elementId()       const {return detId() - channel();}

public:
  virtual double  resolutionEstimate() const;
  double          resolution() const;

  unsigned short  device()     const;
  unsigned short  connection() const;
  unsigned short  block()      const;
  unsigned short  channel()    const;

protected:
  void            calculateAngle();

protected:
  ModuleType     m_type;
  unsigned short m_detId;
  double         m_signalHeight;
  TVector3       m_position;
  TVector3       m_counterPosition;
  double         m_angle;
  double         m_resolution; //!

  ClassDef(Hit, 1);

};

#endif /* Hit_hh */
