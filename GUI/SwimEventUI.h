#ifndef SWIMEVENTUI_H
#define SWIMEVENTUI_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QShortcut>

#include "SwimMeet.h"
#include "Swimmer.h"
#include "ParticipantUI.h"
#include "HockeyGame.h"
#include "Clock.h"

class SwimEventUI : public QWidget
{
    Q_OBJECT
public:
    explicit SwimEventUI(SwimMeet* meet, int eventNumber, HockeyGame* game, QWidget *parent = nullptr);

signals:
    void showLanes(QList<Swimmer> swimmers, QString eventName);
    void showResults(QList<Swimmer> swimmers, QString eventName);
    void showTimes(QList<Swimmer> swimmers, QString eventName);
private slots:
    void prepLaneAssignments();
    void prepLaneResults();
    void prepLaneResultsWithTimes();
    void addTime();
    void setPlaces();
    void resetResultData();
private:
    SwimMeet* meet;
    QList<ParticipantUI*> participantUIs;
    QPushButton laneAssignments, finalResults, enterFinalResults, resetResults;
    QList<QString> times;
    QString places;
    QLineEdit eventName;

    QShortcut* addTimeShortcut;

    Clock* clock;
};

#endif // SWIMEVENTUI_H
