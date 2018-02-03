#ifndef SWIMMER_H
#define SWIMMER_H

#include <QObject>
#include "School.h"
class Swimmer
{
public:
    Swimmer(QString name, School* school);

    QString getName() const;
    void setName(const QString &value);

    School *getSchool() const;
    void setSchool(School *value);

private:
    QString name;
    School* school;
};

#endif // SWIMMER_H
