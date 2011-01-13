#include "TRDEfficiencyPlot.hh"

#include "Setup.hh"
#include "DetectorElement.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "SimpleEvent.hh"
#include "Track.hh"
#include "TrackInformation.hh"
#include "TRDCalculations.hh"

#include <TCanvas.h>
#include <TList.h>
#include <TH2.h>
#include <TAxis.h>
#include <TPaletteAxis.h>

#include <QDebug>

#include <math.h>

TRDEfficiencyPlot::TRDEfficiencyPlot()
  : AnalysisPlot(AnalysisPlot::Occupancy)
  , H2DPlot()
{
  setTitle("TRD efficiency (hits / was on track)");

  TH2D* histogram = new TH2D(qPrintable(title()), qPrintable(title() + ";x / mm;z /mm"),200,-120,120,200,-550, -250);
  setHistogram(histogram);

  //initialize the 2D positions of the trd tubes:
  Setup* setup = Setup::instance();

  DetectorElement* element = setup->firstElement();
  while(element) {
    if (element->type() == DetectorElement::trd){
      //loop over the 16 tubes
      for( int i = 0; i < 16; i++){
        int detID = element->id() | i ;
        //get position of trd tube
        TVector3 pos3D = setup->configFilePosition("trd", detID);
        QVector2D pos2D(pos3D.x(), pos3D.z());
        m_trdChanPositions.insert(detID, pos2D);
      }
    }
    element = setup->nextElement();
  }

  //initialize all needed ellipses
  initializeEllipses();

  //connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(update()));
  //m_updateTimer.start(3000);
}

TRDEfficiencyPlot::~TRDEfficiencyPlot()
{
  qDeleteAll(m_ellipses.values());
  m_ellipses.clear();
}

void TRDEfficiencyPlot::processEvent(const QVector<Hit*>& clustersOnTrack, Track* track, SimpleEvent*)
{
  //check if everything worked and a track has been fit
  if (!track || !track->fitGood())
    return;

  //filter: only use events with 8 tracker hits:
  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::AllTrackerLayers))
    return;

  //loop over all hits and find all clusters on track
  QVector<Cluster*> trdClustersOnTrack;

  //TODO: check for off track hits, atm this is Bastians criteria for on track
  foreach(Hit* clusterHit, clustersOnTrack){
    if (clusterHit->type() == Hit::trd){
      Cluster* cluster = static_cast<Cluster*>(clusterHit);
      //check if event contains trd clusters with more than 2 sub hits
      if (cluster->hits().size() > 2)
        return;

      trdClustersOnTrack << cluster;
    }
  }

  //TODO: loops are not very efficient (maybe use more breaks or smarter searches in rows)

  //now we loop over all trd tubes and calculate the distance between their wire and the track
  //if the tube is hit by the track (defined by a radius a bit smaller than the tube radius)
  //we check wheter the tube shows a signal or not
  QMapIterator<unsigned short, QVector2D> i(m_trdChanPositions);
  while (i.hasNext()) {
    i.next();
    QVector2D pos2D = i.value();
    //get distance
    double distance = TRDCalculations::distanceTrackToWire(pos2D, track);
    //check if tube has been punched by track:
    if (fabs(distance) < 2) {
      //was on track +1
      m_wasOnTrack[i.key()]++;
      //now check wheter the tube has seen a signal:
      foreach(Cluster* cluster, trdClustersOnTrack){
        foreach(Hit* hit, cluster->hits()){
          if ( i.key() == hit->detId()) {
            //tube did see a signal +1
            m_hits[i.key()]++;
          }
        }
      }
      //update histo bin:
      unsigned long hits = m_hits.value(i.key());
      unsigned long wasOnTrack = m_wasOnTrack.value(i.key());
      double value = hits / static_cast<double>(wasOnTrack);
      int bin = histogram()->FindBin(pos2D.x(), pos2D.y());
      histogram()->SetBinContent(bin, value);
    }
  }

}


void TRDEfficiencyPlot::finalize()
{
}

void TRDEfficiencyPlot::draw(TCanvas* c)
{
  //qDebug("Draw");
  H2DPlot::draw(c);

  updateEllipses();

  //loop all map entries:
  QMapIterator<unsigned short, TEllipse*> i(m_ellipses);
  while (i.hasNext()) {
    i.next();

    i.value()->Draw("same");
  }
}

void TRDEfficiencyPlot::updateEllipses()
{
  //qDebug("update ellipses");
  gPad->Update();
  TPaletteAxis* palette = (TPaletteAxis*) histogram()->GetListOfFunctions()->FindObject("palette");

  if(!palette)
    return;

  //loop all map entries:
  QMapIterator<unsigned short, unsigned long> i(m_wasOnTrack);
  while (i.hasNext()) {
    i.next();

    double value = m_hits.value(i.key()) / static_cast<double>(i.value());
    TEllipse* ell = m_ellipses.value(i.key());
    m_mutex.lock();
    ell->SetFillColor(palette->GetValueColor(value));
    m_mutex.unlock();
  }
}

void TRDEfficiencyPlot::update()
{
  static int counter = 0;
  if (counter % 10 == 0) {
    updateEllipses();
  }
  counter++;
}

void TRDEfficiencyPlot::initializeEllipses()
{
  //create for each trd channel an ellipses
  Setup* setup = Setup::instance();

  DetectorElement* element = setup->firstElement();
  while(element) {
    if (element->type() == DetectorElement::trd){
      for(unsigned short tubeNo = 0; tubeNo < 16; tubeNo++){
        unsigned short detID = element->id() | tubeNo;
        TVector3 posTRDChan = setup->configFilePosition("trd",detID);
        TEllipse* ell = new TEllipse(posTRDChan.x(),posTRDChan.z(), 3.0);
        m_ellipses.insert(detID, ell) ;
        ell->SetFillColor(kWhite);
      }
    }
    element = setup->nextElement();
  }
}