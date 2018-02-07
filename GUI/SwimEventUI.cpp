#include "SwimEventUI.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QInputDialog>

SwimEventUI::SwimEventUI(SwimMeet *meet, int eventNumber, HockeyGame *game, QWidget *parent) : QWidget(parent)
{
    laneAssignments.setText("Show Lanes");
    finalResults.setText("Show Final Results");
    enterFinalResults.setText("Input Results");
    resetResults.setText("Reset Results");

    QHBoxLayout* buttons = new QHBoxLayout();
    buttons->addWidget(&laneAssignments);
    buttons->addWidget(&finalResults);
    buttons->addWidget(&enterFinalResults);
    buttons->addWidget(&resetResults);
    addTimeShortcut = new QShortcut("Ctrl+" + QString::number(eventNumber % 10), this);

    QVBoxLayout* widgetLayout = new QVBoxLayout();
    widgetLayout->addWidget(new QLabel("SHORTCUT FOR TIME ENTRY: "+addTimeShortcut->key().toString() ));
    widgetLayout->addLayout(buttons);

    for (int i = 0; i < 8; i++) {
        participantUIs.append(new ParticipantUI(meet, QString::number(i+1)));
    }
    for (int i = 0; i < 8; i++) {
        widgetLayout->addWidget(participantUIs[i]);

    }
    setLayout(widgetLayout);
    eventName.setText("400 YARD BUTTERFLY");

    connect(&laneAssignments, SIGNAL(clicked(bool)), this, SLOT(prepLaneAssignments()));
    connect(&finalResults, SIGNAL(clicked(bool)), this, SLOT(prepLaneResults()));
    connect(addTimeShortcut, SIGNAL(activated()), this, SLOT(addTime()));
    connect(&resetResults, SIGNAL(clicked(bool)), this, SLOT(resetResultData()));
    connect(&enterFinalResults, SIGNAL(clicked(bool)), this, SLOT(setPlaces()));
    addTimeShortcut->setAutoRepeat(false);

    clock = game->getGameClock();
}

void SwimEventUI::prepLaneAssignments()
{
    QList<Swimmer>swimmers;
    for (int i = 0; i < participantUIs.size(); i++) {
        swimmers.append(Swimmer(participantUIs[i]->getName(),participantUIs[i]->getSchool(),
                                participantUIs[i]->getLaneNumber()));
    }
    emit showLanes(swimmers, eventName.text());
}

void SwimEventUI::prepLaneResults()
{
    QList<Swimmer>swimmers;
    for (int i = 0; i < participantUIs.size(); i++) {
        swimmers.append(Swimmer(participantUIs[i]->getName(),participantUIs[i]->getSchool(),
                                participantUIs[i]->getLaneNumber(),places.mid(i,1),
                                times.size() > 0 ? times[places.mid(i,1).toInt()-1] : ""));
    }
    if (times.size() < 0) {
        emit showResults(swimmers, eventName.text());
    } else {
        emit showTimes(swimmers, eventName.text());
    }
}

void SwimEventUI::prepLaneResultsWithTimes()
{
    QList<Swimmer>swimmers;
    for (int i = 0; i < participantUIs.size(); i++) {
        swimmers.append(Swimmer(participantUIs[i]->getName(),participantUIs[i]->getSchool(),
                                participantUIs[i]->getLaneNumber(), times[i]));
    }
    emit showTimes(swimmers, eventName.text());
}

void SwimEventUI::addTime()
{
    QString time = clock->toString();
    if (times.size() < participantUIs.size()) {
        times.append(time);
    }
}

void SwimEventUI::setPlaces()
{
    bool ok;
    int text = QInputDialog::getInt(this, "Enter places by lane", "Places",0,0,999999999,0,&ok);
    if (ok) {
        places=QString::number(text);
    }
}

void SwimEventUI::resetResultData()
{
    times.clear();
    places="";
}

