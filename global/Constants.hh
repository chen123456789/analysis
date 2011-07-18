#ifndef Constants_hh
#define Constants_hh

#include <cmath>

namespace Constants
{

  const double electronMass = 0.000511;
  const double muonMass = 0.105658;
  const double pionMass = 0.139570;
  const double protonMass = 0.938272;
  const double heliumMass = 4.002602*protonMass;

  const double speedOfLight = 299.792458;  // mm/ns
  const double tofBarLength = 395;         // mm
  const double tofBarWidth = 50;           // mm
  const double tofBarHeight = 6;           // mm
  const double tofRefractiveIndex = 1.58;
  const double upperTofPosition = 281.55;  // mm
  const double lowerTofPosition = -531.55; // mm
  const double sigmaSipm = 0.6;            // ns
  const double tofClusterSignalCut = 600;  // ns
  const double minimalTotPerSipm = 17.;
  const double triggerMatchingMinimum = 95.;
  const double triggerMatchingMaximum = 140.;
  const double idealTot = 30.;             //ns
  const int nTofChannels = 64;
  const int nTofBars = 16;
  const int nTofSipmsPerBar = 4;

  const double TRDTubeRadius = 3.0;        //mm

  const int runLength = 600; // seconds

  const double geometricAcceptance = 31.8198/(pow(100, 2));//m^2sr
  const double geometricAcceptanceTof=84.8509/(pow(100, 2));//m^2sr
}

#endif
