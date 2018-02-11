#include "ParticipantUI.h"

#include <QHBoxLayout>
#include <QInputDialog>

ParticipantUI::ParticipantUI(SwimMeet *meet, QString laneNo, HockeyGame *game, bool diving, QString eventName, QWidget *parent) : QWidget(parent)
{
    this->eventName=eventName;
    this->diving=diving;
    QHBoxLayout* widgetLayout = new QHBoxLayout();
    laneNumber.setText(laneNo);
    for (int i = 0; i < meet->getSchools().length(); i++) {
        schoolSelector.addItem(meet->getSchools()[i]->getName());
    }
    lowerThirdButton.setText("Show L3rd");
    addScoreButton.setText("Add Score");
    widgetLayout->addWidget(&laneNumber);
    widgetLayout->addWidget(&schoolSelector);
    widgetLayout->addWidget(&nameField);
    widgetLayout->addWidget(&lowerThirdButton);
    if (diving) widgetLayout->addWidget(&addScoreButton);

    connect(&lowerThirdButton, SIGNAL(clicked(bool)), this, SLOT(prepareLt()));
    connect(&addScoreButton, SIGNAL(clicked(bool)), this, SLOT(addScore()));
    connect(this, SIGNAL(showLt(Swimmer,QString)), game->getLt(), SLOT(prepareSticker(Swimmer,QString)));
    connect(this, SIGNAL(showDiverLt(Diver,QString)), game->getLt(), SLOT(prepareDiver(Diver,QString)));

    setLayout(widgetLayout);
    this->meet = meet;
}

QString ParticipantUI::getName() const
{
    return nameField.text();
}

School* ParticipantUI::getSchool() const
{
    return meet->getSchools()[schoolSelector.currentIndex()];
}

QString ParticipantUI::getLaneNumber() const
{
    return laneNumber.text();
}

Diver ParticipantUI::getDiver()
{
    diver.setName(getName());
    diver.setSchool(getSchool());
    diver.setDiverNumber(getLaneNumber());
    return diver;
}

void ParticipantUI::prepareLt()
{
    if (!diving) {
        Swimmer swimmer(nameField.text(), meet->getSchools()[schoolSelector.currentIndex()],laneNumber.text());
        emit showLt(swimmer, eventName);
    } else {
        diver.setName(getName());
        diver.setSchool(getSchool());
        diver.setDiverNumber(getLaneNumber());
        emit showDiverLt(diver, eventName);
    }
}

void ParticipantUI::updateEventName(QString name)
{
    this->eventName=name;
}

void ParticipantUI::addScore()
{
    bool ok;
    double text = QInputDialog::getDouble(this, "Enter score", "Score",0,0,100,2,&ok);
    if (ok) {
        diver.addScore(text);
    }
}
