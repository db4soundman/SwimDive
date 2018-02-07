#include "Swimmer.h"

Swimmer::Swimmer()
{

}

Swimmer::Swimmer(QString name, School *school)
{
    this->name=name;
    this->school=school;
}

Swimmer::Swimmer(QString name, School *school, QString laneNumber, QString place, QString time)
{
    this->name=name;
    this->school=school;
    this->laneNumber=laneNumber;
    this->place=place;
    this->time=time;
}

QString Swimmer::getName() const
{
    return name;
}

void Swimmer::setName(const QString &value)
{
    name = value;
}

School *Swimmer::getSchool() const
{
    return school;
}

void Swimmer::setSchool(School *value)
{
    school = value;
}

QString Swimmer::getLaneNumber() const
{
    return laneNumber;
}

void Swimmer::setLaneNumber(const QString &value)
{
    laneNumber = value;
}

QString Swimmer::getPlace() const
{
    return place;
}

void Swimmer::setPlace(const QString &value)
{
    place = value;
}

QString Swimmer::getTime() const
{
    return time;
}

void Swimmer::setTime(const QString &value)
{
    time = value;
}
