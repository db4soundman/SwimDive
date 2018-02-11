#include "Diver.h"
#include <algorithm>

Diver::Diver()
{

}

Diver::Diver(QString name, School *school)
{
    this->name=name;
    this->school=school;
}

Diver::Diver(QString name, School *school, QString diverNumber)
{
    this->name=name;
    this->school=school;
    this->diverNumber=diverNumber;
}

QString Diver::getName() const
{
    return name;
}

void Diver::setName(const QString &value)
{
    name = value;
}

School *Diver::getSchool() const
{
    return school;
}

void Diver::setSchool(School *value)
{
    school = value;
}

QString Diver::getDiverNumber() const
{
    return diverNumber;
}

void Diver::setDiverNumber(const QString &value)
{
    diverNumber = value;
}

QList<double> Diver::getScores() const
{
    return scores;
}

void Diver::setScores(const QList<double> &value)
{
    scores = value;
}

double Diver::getTotalScore()
{
    return std::accumulate(scores.begin(), scores.end(),0.0);
}

void Diver::addScore(double score)
{
    scores.append(score);
}

bool Diver::operator !=(Diver a)
{
    return a.getName() != name;
}
