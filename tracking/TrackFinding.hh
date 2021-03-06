#ifndef TrackFinding_hh
#define TrackFinding_hh

#include <QVector>

class Hit;
class TH2D;
class Track;

class TrackFinding {
public:
  TrackFinding();
  virtual ~TrackFinding();

  void setSlopeBins(int nBins) {m_slopeBins = nBins;}
  void setOffsetBins(int nBins) {m_offsetBins = nBins;}
  void setTrackerPull(double pull) {m_trackerPull = pull;}
  void setTrdPull(double pull) {m_trdPull = pull;}
  void setTofPull(double pull) {m_tofPull = pull;}

  QVector<Hit*> findTrack(const QVector<Hit*>& hits);

  TH2D* trackFindingHist() const {return m_trackFindingHist;}

  static bool isInCorridor(const Track* track, Hit* hit, double maxPull);

private:
  double m_maxSlope;
  double m_maxOffset;
  int m_slopeBins;
  int m_offsetBins;

  double m_trackerPull;
  double m_trdPull;
  double m_tofPull;

  TH2D* m_trackFindingHist;
  static int s_histCounter;

};

#endif /* TrackFinding_hh */
