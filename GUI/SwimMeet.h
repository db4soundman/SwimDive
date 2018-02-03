#ifndef SWIMMEET_H
#define SWIMMEET_H

#include <QObject>

#include "School.h"

class SwimMeet
{
public:
    SwimMeet();

    QList<School> getSchools() const;
    void setSchools(const QList<School> &value);

private:
    QList<School> schools;
};

#endif // SWIMMEET_H
