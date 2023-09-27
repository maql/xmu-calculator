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
    static QRegularExpression symbolRegExp("[A-Z][a-z]{0,2}");
    static QRegularExpression doubleRegExp("[0-9]*\\.[0-9]+");
    static QRegularExpression intRegExp("[0-9]+");
    QRegularExpressionMatch match;

    if (line.startsWith('-')) {
        return false;
    }
    if (line.indexOf(symbolRegExp, 4, &match) == 4) {
        symbol = match.captured(0);
    }
    if (line.indexOf(intRegExp, 8, &match) == 8) {
        imass = match.captured(0).toInt();
    } else {
        return false;
    }
    if (line.indexOf(doubleRegExp, 13, &match) == 13) {
        mass = match.captured(0).toDouble();
    } else {
        return false;
    }

    auto posAbundance = line.indexOf(doubleRegExp, 32, &match);
    if (posAbundance == 32 || posAbundance == 33) {
        abundance = match.captured(0).toDouble();
    } else {
        abundance = 0.0;
    }
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
