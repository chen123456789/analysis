#include "DataInterface.hh"

#include "AlignmentMatrix.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "SimpleEvent.hh"
#include "Layer.hh"
#include "CenteredBrokenLine.hh"
#include "DataChain.hh"
#include "TrackInformation.hh"
#include "TrackFinding.hh"
#include "Manager.hh"
#include "Setup.hh"
#include "Corrections.hh"

#include "millepede.h"

#include <iostream>

DataInterface::DataInterface() :
  m_chain(new DataChain),
  m_trackFinding(new TrackFinding),
  m_corrections(new Corrections())
{
}

DataInterface::~DataInterface()
{
  delete m_chain;
  delete m_trackFinding;
  delete m_corrections;
}

void DataInterface::addFiles(const char* listName)
{
  m_chain->addFileList(listName);
}

void DataInterface::process(AlignmentMatrix* matrix)
{
  unsigned int nEntries = m_chain->nEntries();
  std::cout << std::endl;
  std::cout << "+----------------------------------------------------------------------------------------------------+" << std::endl;
  std::cout << "| Processing:                                                                                        |" << std::endl;
  std::cout << "| 0%     10%       20%       30%       40%       50%       60%       70%       80%       90%     100%|" << std::endl;
  std::cout << "|.........|.........|.........|.........|.........|.........|.........|.........|.........|..........|" << std::endl;
  std::cout << "|" << std::flush;
  int iFactors = 0;

  for (unsigned int i = 0; i < nEntries; i++) {
    SimpleEvent* event = m_chain->event(i);
    
    // retrieve data
    QVector<Hit*> clusters;
    QVector<Hit*> hits = QVector<Hit*>::fromStdVector(event->hits());
    if (event->contentType() == SimpleEvent::Clusters)
      clusters = hits;
    else
      clusters = Setup::instance()->generateClusters(hits);

    // corrections (previous alignment, time shift, ...)
    m_corrections->apply(clusters);

    // track finding
    QVector<Hit*> trackClusters = m_trackFinding->findTrack(clusters);

    // fit in millepede
    CenteredBrokenLine track;
    if (track.process(trackClusters)) {
      TrackInformation::Flags flags = track.information()->flags();
      if ( (flags & TrackInformation::AllTrackerLayers) &&
          !(flags & TrackInformation::MagnetCollision) ) {
        matrix->fillMatrixFromTrack(&track);
        FITLOC();
      }
    }
    
    // delete clusters if necessary
    if (event->contentType() == SimpleEvent::RawData)
      qDeleteAll(clusters);

    if ( i > iFactors*nEntries/100. ) {
      std::cout << "#" << std::flush;
      iFactors++;
    }
  }

  std::cout << "|" << std::endl;
  std::cout << "+----------------------------------------------------------------------------------------------------+" << std::endl;
}
