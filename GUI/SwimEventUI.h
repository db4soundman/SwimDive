#ifndef SWIMEVENTUI_H
#define SWIMEVENTUI_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QShortcut>
#include <QCheckBox>

#include "SwimMeet.h"
#include "Swimmer.h"
#include "ParticipantUI.h"
#include "HockeyGame.h"
#include "Clock.h"

class SwimEventUI : public QWidget
{
    Q_OBJECT
public:
    explicit SwimEventUI(SwimMeet* meet, int eventNumber, HockeyGame* game, bool diving=false, QWidget *parent = nullptr);

    void sendEventToScoreboard();

signals:
    void showLanes(QList<Swimmer> swimmers, QString eventName);
    void showResults(QList<Swimmer> swimmers, QString eventName);
    void showDiveLanes(QList<Diver> swimmers, QString eventName);
    void showDiveResults(QList<Diver> swimmers, QString eventName);
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
    QCheckBox prelimChamp;
    bool diving;

    QShortcut* addTimeShortcut;

    Clock* clock;

    HockeyGame* game;
};

#endif // SWIMEVENTUI_H
