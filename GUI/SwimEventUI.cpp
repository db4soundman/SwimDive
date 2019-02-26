#include "SwimEventUI.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QInputDialog>
#include <algorithm>
#include <functional>


SwimEventUI::SwimEventUI(SwimMeet *meet, int eventNumber, HockeyGame *game, bool diving, QWidget *parent) : QWidget(parent)
{
    this->game=game;
    if (!diving) connect(&eventName, SIGNAL(textChanged(QString)), game->getSb(),SLOT(changeTopBarText(QString)));
    laneAssignments.setText(diving ? "Show Order":"Show Lanes");
    finalResults.setText("Show Final Results");
    enterFinalResults.setText("Input Results");
    resetResults.setText("Reset Results");
    //eventName.setText("400 YARD BUTTERFLY");
    prelimChamp.setText("Championship");
    this->diving=diving;
    QHBoxLayout* buttons = new QHBoxLayout();
    buttons->addWidget(&laneAssignments);
    buttons->addWidget(&finalResults);
    if (!diving) buttons->addWidget(&enterFinalResults);
    if (!diving) buttons->addWidget(&resetResults);
    buttons->addWidget(&prelimChamp);
    addTimeShortcut = new QShortcut("Ctrl+" + QString::number(eventNumber % 10), this);

    QVBoxLayout* widgetLayout = new QVBoxLayout();
    widgetLayout->addWidget(&eventName);
    if (!diving) {
        widgetLayout->addWidget(new QLabel("SHORTCUT FOR TIME ENTRY: "+addTimeShortcut->key().toString() ));
    }
    else {
        addTimeShortcut->setEnabled(false);
    }
    widgetLayout->addLayout(buttons);

    for (int i = 0; i < 8; i++) {
        participantUIs.append(new ParticipantUI(meet, QString::number(i+1),game, diving,eventName.text()));
        connect(&eventName, SIGNAL(textChanged(QString)), participantUIs.last(), SLOT(updateEventName(QString)));
    }
    for (int i = 0; i < 8; i++) {
        widgetLayout->addWidget(participantUIs[i]);

    }

    connect(&prelimChamp, SIGNAL(toggled(bool)), game->getSb(), SLOT(setPeriod(bool)));
    setLayout(widgetLayout);


    connect(&laneAssignments, SIGNAL(clicked(bool)), this, SLOT(prepLaneAssignments()));
    connect(&finalResults, SIGNAL(clicked(bool)), this, SLOT(prepLaneResults()));
    connect(addTimeShortcut, SIGNAL(activated()), this, SLOT(addTime()));
    connect(&resetResults, SIGNAL(clicked(bool)), this, SLOT(resetResultData()));
    connect(&enterFinalResults, SIGNAL(clicked(bool)), this, SLOT(setPlaces()));
    addTimeShortcut->setAutoRepeat(false);

    clock = game->getGameClock();
}

void SwimEventUI::sendEventToScoreboard()
{
    game->getSb()->changeTopBarText(eventName.text());
    game->getSb()->setPeriod(prelimChamp.isChecked());
}

void SwimEventUI::prepLaneAssignments()
{
    if (!diving) {
        QList<Swimmer>swimmers;
        for (int i = 0; i < participantUIs.size(); i++) {
            swimmers.append(Swimmer(participantUIs[i]->getName(),participantUIs[i]->getSchool(),
                                    participantUIs[i]->getLaneNumber()));
        }
        emit showLanes(swimmers, eventName.text());
    } else {
        QList<Diver> divers;
        for (int i = 0; i < participantUIs.size(); i++) {
            divers.append(participantUIs[i]->getDiver());
        }
        emit showDiveLanes(divers,eventName.text());
    }
}

void SwimEventUI::prepLaneResults()
{
    if (!diving) {
        QList<Swimmer>swimmers;
        for (int i = 0; i < participantUIs.size(); i++) {
            swimmers.append(Swimmer(participantUIs[i]->getName(),participantUIs[i]->getSchool(),
                                    participantUIs[i]->getLaneNumber(),places.mid(i,1),
                                    times.size() > 0 && i < times.size()  && i < places.length() ? times[places.mid(i,1).toInt()-1] : ""));
        }
        if (times.size() < 0) {
            emit showResults(swimmers, eventName.text());
        } else {
            emit showTimes(swimmers, eventName.text());
        }
    } else {
        QList<Diver> divers;
        for (int i = 0; i < participantUIs.size(); i++) {
            divers.append(participantUIs[i]->getDiver());
        }
        emit showDiveResults(divers,eventName.text());
    }
}

void SwimEventUI::prepLaneResultsWithTimes()
{
    QList<Swimmer>swimmers;
    for (int i = 0; i < participantUIs.size(); i++) {
        swimmers.append(Swimmer(participantUIs[i]->getName(),participantUIs[i]->getSchool(),
                                participantUIs[i]->getLaneNumber(), times.size() < i ? times[i] : ""));
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

