#ifndef Likelihood_hh
#define Likelihood_hh

#include "Enums.hh"
#include "KineticVariable.hh"

#include <QMap>
#include <QVector>
#include <QDebug>

class Particle;
class LikelihoodPDF;

class Likelihood {
public:
  Likelihood();
  ~Likelihood();
  Enums::Particles particles() const;
  Enums::KineticVariable measuredValueType() const;
  Enums::LikelihoodVariable likelihoodVariableType() const;
  static Likelihood* newLikelihood(Enums::LikelihoodVariable);
  virtual LikelihoodPDF* pdf(const KineticVariable&) const;

  virtual double min() const = 0;
  virtual double max() const = 0;
  virtual int numberOfParameters() const = 0;
  virtual double eval(Particle*, const KineticVariable& hypothesis, bool* goodInterpolation = 0) const = 0;
  virtual double eval(double measuredValue, const KineticVariable& hypothesis, bool* goodInterpolation = 0) const = 0;
protected:
  typedef QVector<double> ParameterVector;
  typedef QMap<double, ParameterVector> MomentumMap;
  typedef QMap<Enums::Particle, MomentumMap> ParticleMap;

  virtual MomentumMap::ConstIterator end(Enums::Particle) const;
  virtual MomentumMap::ConstIterator lowerNode(const KineticVariable&) const;
  virtual MomentumMap::ConstIterator upperNode(const KineticVariable&) const;
  virtual ParameterVector defaultParameters() const;
  virtual void loadNodes();
  virtual ParameterVector linearInterpolation(const KineticVariable&, bool* goodInterpolation = 0) const;

  Enums::Particles m_particles;
  Enums::KineticVariable m_measuredValueType;
  Enums::LikelihoodVariable m_likelihoodVariableType;
private:
  ParticleMap m_nodes;
};

#endif
