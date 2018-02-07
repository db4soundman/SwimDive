#include "SwimMeet.h"

SwimMeet::SwimMeet()
{

}

QList<School *> SwimMeet::getSchools() const
{
    return schools;
}

void SwimMeet::setSchools(const QList<School*> &value)
{
    schools = value;
}
