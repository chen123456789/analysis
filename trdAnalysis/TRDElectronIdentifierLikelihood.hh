#ifndef TRDELECTRONIDENTIFIERLIKELIHOOD_HH
#define TRDELECTRONIDENTIFIERLIKELIHOOD_HH

#include "TRDElectronIdentifier.hh"

class TF1;

class TRDElectronIdentifierLikelihood : public TRDElectronIdentifier
{
public:
    TRDElectronIdentifierLikelihood();
    bool isElectronish(const QVector<Hit*>& hits, const Particle* particle, const SimpleEvent* event, bool &ok);
private:
    const TF1* m_LHFunctionTR;
    QVector<const TF1*> m_LHFunctionNonTR;
};

#endif // TRDELECTRONIDENTIFIERLIKELIHOOD_HH
