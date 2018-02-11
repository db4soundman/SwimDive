#ifndef DIVER_H
#define DIVER_H

#include <QObject>
#include "School.h"
class Diver
{
public:
    Diver();
    Diver(QString name, School* school);
    Diver(QString name, School* school, QString diverNumber);

    QString getName() const;
    void setName(const QString &value);

    School *getSchool() const;
    void setSchool(School *value);

    QString getDiverNumber() const;
    void setDiverNumber(const QString &value);

    QList<double> getScores() const;
    void setScores(const QList<double> &value);

    double getTotalScore();

    void addScore(double score);

    bool operator !=(Diver a);

private:
    QString name;
    School* school;
    QString diverNumber;
    QList<double> scores;
};

#endif // DIVER_H
