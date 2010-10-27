#ifndef TOFSipmHit_hh
#define TOFSipmHit_hh

#include "Hit.hh"

#include <vector>
#include <stdint.h>

class TOFSipmHit : public Hit {
public:
  enum EdgeType {LeadingEdge, TrailingEdge};

public:
  TOFSipmHit();
  virtual ~TOFSipmHit();

  void processTDCHits();

  double startTime() const {return m_startTime;}
  double timeOverThreshold() const {return m_timeOverThreshold;}
  int channel() const {return m_channel;}
  int numberOfLevelChanges() const {return m_levelChanges.size();}

  double time(int) const;
  EdgeType edgeType(int) const;
  int channel(int) const;
  int spillNumber(int) const;
  void setLevelChange(int, uint32_t);
  
  void addLevelChange(uint32_t);
	
private:
  static int channelFromData(uint32_t);
  static double timeFromData(uint32_t);
  static bool earlierThan(uint32_t, uint32_t);
    
  int m_channel;
  double m_startTime;
  double m_timeOverThreshold;
  std::vector<uint32_t> m_levelChanges;

  ClassDef( TOFSipmHit, 1 );
};

#endif
