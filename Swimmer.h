#ifndef SWIMMER_H
#define SWIMMER_H

#include <QObject>
#include "School.h"
class Swimmer
{
public:
    Swimmer();
    Swimmer(QString name, School* school);
    Swimmer(QString name, School* school, QString laneNumber, QString place="", QString time="");

    QString getName() const;
    void setName(const QString &value);

    School *getSchool() const;
    void setSchool(School *value);

    QString getLaneNumber() const;
    void setLaneNumber(const QString &value);

    QString getPlace() const;
    void setPlace(const QString &value);

    QString getTime() const;
    void setTime(const QString &value);

private:
    QString name;
    School* school;
    QString laneNumber;
    QString place;
    QString time;
};

#endif // SWIMMER_H
