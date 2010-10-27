#include "Matrix.hh"

#include "Manager.hh"
#include "Strategy.hh"
#include "Hit.hh"
#include "Track.hh"
#include "Parameters.hh"
#include "Setup.hh"
#include "DetectorElement.hh"
#include "millepede.h"

#include <TMatrixD.h>

#include <cmath>

Matrix::Matrix() :
  m_nGlobal(0),
  m_nLocal(0),
  m_globalDerivatives(0),
  m_localDerivatives(0)
{
}

Matrix::~Matrix()
{
  delete[] m_globalDerivatives;
  delete[] m_localDerivatives;
}

void Matrix::init()
{
  Manager* manager = Manager::instance();
  Strategy* strategy = manager->strategy();

  m_nGlobal = strategy->numberOfGlobalParameters();
  m_nLocal  = strategy->numberOfLocalParameters();;

  delete[] m_globalDerivatives;
  delete[] m_localDerivatives;
  m_globalDerivatives = new float[m_nGlobal];
  m_localDerivatives = new float[m_nLocal];

  resetArrays();
}

void Matrix::resetArrays()
{
  for (unsigned int i = 0; i < m_nGlobal; i++)
    m_globalDerivatives[i] = 0.;
  for (unsigned int i = 0; i < m_nLocal; i++)
    m_localDerivatives[i] = 0.;
}

void Matrix::fillMatrixFromTrack(Track* track)
{
  resetArrays();

  QVector<Hit*> hits = track->hits();
  
  foreach(Hit* hit, hits) {

    // position
    TVector3 pos = hit->position();
    float fx = pos.x();
    float fy = pos.y();
    float fz = pos.z();
    float z0 = 0.0;
    float k  = fz - z0;

    // angle
    float angle = hit->angle();
    angle += M_PI/2.;
    bool  useTangens = fabs(angle) < M_PI/4. ? true : false;
    float xi = useTangens ? tan(angle) : 1./tan(angle);

    // specify resolution
    double sigmaV = hit->resolutionEstimate();

    // detector ID
    unsigned short detId = hit->detId() - hit->channel();
    unsigned int index = Manager::instance()->parameters()->indexForDetId(detId);

    // Rot is the matrix that maps u,v, to x,y (i.e. the backward rotation)
    TMatrixD Rot(2,2); 
    Rot(0,0) = cos(angle);
    Rot(0,1) = sin(angle);
    Rot(1,0) = -sin(angle);
    Rot(1,1) = cos(angle);
    TMatrixD V1(2,2);
    V1(0,0) = 0.;
    V1(0,1) = 0.;
    V1(1,0) = 0.;
    V1(1,1) = sigmaV*sigmaV;
    TMatrixD RotTrans(2,2);
    RotTrans.Transpose(Rot);
    TMatrixD V2(2,2);
    V2 = Rot * V1 * RotTrans;
    TMatrixD Lin(1,2);
    if (useTangens) {
      Lin(0,0) = -xi;
      Lin(0,1) = 1.;
    }
    else {
      Lin(0,0) = 1.;
      Lin(0,1) = -xi;
    }
    TMatrixD LinTrans(2,1);
    LinTrans.Transpose(Lin);
    TMatrixD V3 = TMatrixD(1,1);
    V3 = Lin * V2 * LinTrans;

    float y,sigma;
    if (useTangens) {
      y = -xi*fx + fy;
      sigma = sqrt(V3(0,0));

      // derivative for Delta_x!
      m_globalDerivatives[index] = -xi;
      
      // float deltaX = parameters->GetParameter(shiftIndex);
      // float x0 = track->x0();
      // float lambda_x = track->slopeX();
      // m_globalDerivatives[rotIndex] = -deltaX - x0 - k*lambda_x + fx;

      if (m_nLocal == 4) {
        m_localDerivatives[0] = -xi;
        m_localDerivatives[1] = 1.;
        m_localDerivatives[2] = -k*xi;
        m_localDerivatives[3] = k;
      }
      else if (m_nLocal == 2) {
        m_localDerivatives[0] = 1.;
        m_localDerivatives[1] = k;
      }
    }
    else {
      y = fx - xi*fy;
      sigma = sqrt(V3(0,0));

      // derivative for Delta_x!
      m_globalDerivatives[index] = 1.;

      // float deltaY = parameters->GetParameter(shiftIndex);
      // float y0 = track->y0();
      // float lambda_y = track->slopeY();
      // m_globalDerivatives[rotIndex] = -deltaY - y0 - k*lambda_y + fy;
       
      if (m_nLocal == 4) {
        m_localDerivatives[0] = 1.;
        m_localDerivatives[1] = -xi;
        m_localDerivatives[2] = k;
        m_localDerivatives[3] = -k*xi;
      }
      else if (m_nLocal == 2) {
        m_localDerivatives[0] = 1.;
        m_localDerivatives[1] = k;
      }
    }

    EQULOC(m_globalDerivatives, m_localDerivatives, y, sigma);
  }
}
