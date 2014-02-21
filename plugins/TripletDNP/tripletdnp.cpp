#include "tripletdnp.h"

#include "frequencycalculator.h"
#include "concentrationcalculator.h"

TripletDNP::TripletDNP()
{
}

QList<QObject *> TripletDNP::plugins()
{
    QList<QObject *> plugins;
    plugins.append(new FrequencyCalculator);
    plugins.append(new ConcentrationCalculator);
    return plugins;
}
