#include "Plotter.hh"

#include "DataChain.hh"
#include "Cluster.hh"
#include "TOFCluster.hh"
#include "TOFSipmHit.hh"
#include "SimpleEvent.hh"
#include "BrokenLine.hh"
#include "StraightLine.hh"
#include "Layer.hh"
#include "TrackFinding.hh"
#include "Setup.hh"
#include "DataDescription.hh"
#include "PlotHits.hh"

#include <TCanvas.h>
#include <TH2I.h>
#include <TChain.h>
#include <TFile.h>
#include <TList.h>

#include <QVector>

#include <fstream>
#include <iostream>

Plotter::Plotter() :
  m_chain(new DataChain),
  m_trackFinding(new TrackFinding)
{
}

Plotter::~Plotter()
{
  delete m_chain;
  delete m_trackFinding;
}

void Plotter::addFiles(const char* listName)
{
  m_chain->addFiles(listName);
}

void Plotter::process()
{
  unsigned int nEntries = m_chain->nEntries();

  std::cout << std::endl;
  std::cout << "+----------------------------------------------------------------------------------------------------+" << std::endl;
  std::cout << "| Processing:                                                                                        |" << std::endl;
  std::cout << "| 0%     10%       20%       30%       40%       50%       60%       70%       80%       90%     100%|" << std::endl;
  std::cout << "|.........|.........|.........|.........|.........|.........|.........|.........|.........|..........|" << std::endl;
  std::cout << "|" << std::flush;
  int iFactors = 0;

  Setup* setup = Setup::instance();
  PlotHits pc;

  for (unsigned int i = 0; i < 100; i++) {
    SimpleEvent* event = m_chain->event(i);

    // vector of all hits in this event
    QVector<Hit*> hits = QVector<Hit*>::fromStdVector(event->hits());
    QVector<Hit*> clusters;
    foreach(Cluster* cluster, setup->generateClusters(hits))
      clusters.push_back(cluster);

    // plot before trackfinding
    pc.plot(clusters);

    // track finding
    clusters = m_trackFinding->findTrack(clusters);

    // plot after trackfinding including track
    BrokenLine track;
    if (track.fit(clusters))
      pc.plot(clusters, &track);
    else
      pc.plot(clusters);

    // delete clusters in case they were allocated dynamically
    setup->deleteClusters();

    if ( i > iFactors*nEntries/100. ) {
      std::cout << "#" << std::flush;
      iFactors++;
    }

  } // loop over entries

  std::cout << "|" << std::endl;
  std::cout << "+----------------------------------------------------------------------------------------------------+" << std::endl;

}

