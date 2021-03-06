#include "BrokenLine.hh"

#include <TVectorD.h>
#include <TMatrixD.h>

#include "BrokenLineMatrix.hh"
#include "Hit.hh"
#include "Constants.hh"

#include <cmath>
#include <iostream>

BrokenLine::BrokenLine() :
  Track(),
  m_upperX0(0),
  m_lowerX0(0),
  m_upperSlopeX(0),
  m_lowerSlopeX(0),
  m_y0(0),
  m_slopeY(0),
  m_zIntersection(0)
{
  m_type = Enums::BrokenLine;
  m_matrix = new BrokenLineMatrix;
}

BrokenLine::~BrokenLine()
{
  delete m_matrix;
}

void BrokenLine::retrieveFitResults()
{
  double* solution = m_matrix->solution();

  // return information from the fit.
  m_upperX0       = solution[0];
  m_lowerX0       = solution[1];
  m_y0            = solution[2];
  m_upperSlopeX   = solution[3];
  m_lowerSlopeX   = solution[4];
  m_slopeY        = solution[5];
  m_chi2          = m_matrix->chi2();
  m_ndf           = m_matrix->ndf();
  m_zIntersection = (m_upperX0 - m_lowerX0) / (m_lowerSlopeX - m_upperSlopeX);

  if (m_verbose > 0) {
    std::cout << "--------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << " results of broken line track fit: chi2/ndf      = " << m_chi2        << "/" << m_ndf << std::endl;
    std::cout << "                                     upperX0     = " << m_upperX0     << " mm" <<std::endl;
    std::cout << "                                     lowerX0     = " << m_lowerX0     << " mm" <<std::endl;
    std::cout << "                                     y0          = " << m_y0          << " mm" <<std::endl;
    std::cout << "                                     upperSlopeX = " << m_upperSlopeX << std::endl;
    std::cout << "                                     lowerSlopeX = " << m_lowerSlopeX << std::endl;
    std::cout << "                                     slopeY      = " << m_slopeY      << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------" << std::endl;
  }
}

double BrokenLine::x(double z) const
{
  if (z > m_zIntersection)
    return m_upperX0 + z*m_upperSlopeX;
  return m_lowerX0 + z*m_lowerSlopeX;
}

double BrokenLine::y(double z) const
{
  return m_y0 + z*m_slopeY;
}

double BrokenLine::slopeX(double z) const
{
  if (z > m_zIntersection)
    return m_upperSlopeX;
  return m_lowerSlopeX;
}

double BrokenLine::slopeY(double) const
{
  return m_slopeY;
}

double BrokenLine::trackLength() const
{
  const TVector3& upperPoint = position(Constants::upperTofPosition);
  const TVector3& middlePoint = position(m_zIntersection);
  const TVector3& lowerPoint = position(Constants::lowerTofPosition);
  return (upperPoint-middlePoint).Mag() + (middlePoint-lowerPoint).Mag();
}
