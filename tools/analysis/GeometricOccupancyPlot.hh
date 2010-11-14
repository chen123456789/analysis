#ifndef GeometricOccupancyPlot_hh
#define GeometricOccupancyPlot_hh

#include "H2DPlot.hh"

class GeometricOccupancyPlot : public H2DPlot {
  public:
    explicit GeometricOccupancyPlot(double zPosition);
    virtual ~GeometricOccupancyPlot();
    virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
    virtual void finalize();
  private:
    double m_zPosition;
};

#endif