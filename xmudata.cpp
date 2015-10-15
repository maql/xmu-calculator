#include <QStringList>
#include <QFile>
#include <QTextStream>

#include "xmudata.h"


Isotope::Isotope(int im, const QString &sym)
{
    imass = im;
    symbol = sym;
}


bool Isotope::operator<(const Isotope &other) const
{
    return (this->imass < other.imass)
        || ((this->imass == other.imass) && (this->symbol < other.symbol));
}


IsotopeAbundance::IsotopeAbundance()
{
    imass = 0;
    abundance = 0.0;
}


void IsotopeAbundance::update(int im, double abund)
{
    if (abund > abundance)
    {
        imass = im;
        abundance = abund;
    }
}


XmuData::XmuData()
{
}


XmuData::XmuData(const QString &fileName)
{
    readFromTextFile(fileName);
}


double XmuData::getXmu(int imass, const QString &symbol) const
{
    if (imass == 0) imass = getDefaultImass(symbol);
    Isotope isotope(imass, symbol);
    return isotopeMass[isotope];
}


int XmuData::getDefaultImass(const QString &symbol) const
{
    return defaultIsotope[symbol].imass;
}


bool XmuData::parseLine(const QString &line, QString &symbol,
                        int &imass, double &mass, double &abundance)
{
    QRegExp symbolRegExp("[A-Z][a-z]{0,2}");
    QRegExp doubleRegExp("[0-9]*\\.[0-9]+");
    QRegExp intRegExp("[0-9]+");

    if (line.startsWith('-')) return false;
    if (line.indexOf(symbolRegExp, 4) == 4) symbol = symbolRegExp.cap();
    if (line.indexOf(intRegExp, 8) == 8) imass = intRegExp.cap().toInt();
    else return false;
    if (line.indexOf(doubleRegExp, 13) == 13)
        mass = doubleRegExp.cap().toDouble();
    else return false;
    if (line.indexOf(doubleRegExp, 32) <= 33)
        abundance = doubleRegExp.cap().toDouble();
    else abundance = 0.0;
    return true;
}


void XmuData::readFromTextFile(const QString &fileName)
{
    QFile data(fileName);
    if (data.open(QFile::ReadOnly))
    {
        QTextStream in(&data);
        QStringList lines = in.readAll().split('\n');
        
        QStringList::const_iterator i;
        QString symbol;
        int imass;
        double mass, abundance;
        
        for (i = lines.constBegin(); i != lines.constEnd(); ++i)
        {
            if (parseLine(*i, symbol, imass, mass, abundance))
            {
                Isotope isotope(imass, symbol);
                isotopeMass[isotope] = mass;
                defaultIsotope[symbol].update(imass, abundance);
            }
        }
    }
}
