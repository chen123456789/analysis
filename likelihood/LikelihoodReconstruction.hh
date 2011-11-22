#ifndef LikelihoodReconstruction_hh
#define LikelihoodReconstruction_hh

#include "Reconstruction.hh"
#include "Hypothesis.hh"

class AnalyzedEvent;
class Likelihood;
class TMultiGraph;
class LogLikelihoodFunction;
namespace ROOT {namespace Math {class BrentMinimizer1D;}}

class LikelihoodReconstruction : public Reconstruction {
public:
  LikelihoodReconstruction(Enums::LikelihoodVariables, Enums::Particles, bool additionalInformation = false);
  virtual ~LikelihoodReconstruction();
  virtual void identify(AnalyzedEvent*);
  virtual TMultiGraph* graph() const;
private:
  double eval(const AnalyzedEvent*, const Hypothesis&, bool* goodInterpolation = 0) const;
  double rootFunctorPointer(const double* momentum);
  ROOT::Math::BrentMinimizer1D* m_lhMinimizer;
  LogLikelihoodFunction* m_lhFunction;
};

#endif