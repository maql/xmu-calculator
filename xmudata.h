#ifndef XMUDATA_H
#define XMUDATA_H

#include <QMap>
#include <QString>
#include <QRegularExpression>


class Isotope
{
public:
    Isotope(int im, const QString &sym);
    bool operator<(const Isotope &other) const;

private:
    int imass;
    QString symbol;
};


class IsotopeAbundance
{
public:
    IsotopeAbundance();
    void update(int im, const double abund);
    int imass;

private:
    double abundance;
};


class XmuData
{
public:
    XmuData();
    XmuData(const QString &fileName);
    void readFromTextFile(const QString &fileName);
    double getXmu(int imass, const QString &symbol) const;
    int getDefaultImass(const QString &symbol) const;

private:
    bool parseLine(const QString &line, QString &symbol,
                   int &imass, double &mass, double &abundance);
    QMap<Isotope, double> isotopeMass;
    QMap<QString, IsotopeAbundance> defaultIsotope;
};

#endif
