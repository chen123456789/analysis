#ifndef HeightTimePlot_hh
#define HeightTimePlot_hh

#include "SensorTimePlot.hh"

#include <QDateTime>

class HeightTimePlot : public SensorTimePlot {
public:
  HeightTimePlot(QDateTime first, QDateTime last);
  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
};

#endif
