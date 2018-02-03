#include "Swimmer.h"

Swimmer::Swimmer(QString name, School *school)
{
    this->name=name;
    this->school=school;
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
