#ifndef TOTTemperatureCorrelationPlot_hh
#define TOTTemperatureCorrelationPlot_hh

#include "AnalysisPlot.hh"
#include "H2DProjectionPlot.hh"
#include "SensorTypes.hh"

#include <QVector>
#include <QMap>

class Hit;
class Particle;
class SimpleEvent;

class TOTTemperatureCorrelationPlot : public AnalysisPlot, public H2DProjectionPlot {
public:
  TOTTemperatureCorrelationPlot(unsigned int tofChannel);
  ~TOTTemperatureCorrelationPlot();
  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
private:
  unsigned short m_id;
};

#endif /* TOTTemperatureCorrelationPlot_hh */
