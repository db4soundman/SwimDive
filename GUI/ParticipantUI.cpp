#include "ParticipantUI.h"

#include <QHBoxLayout>

ParticipantUI::ParticipantUI(SwimMeet *meet, QString laneNo, HockeyGame *game, QString eventName, QWidget *parent) : QWidget(parent)
{
    this->eventName=eventName;
    QHBoxLayout* widgetLayout = new QHBoxLayout();
    laneNumber.setText(laneNo);
    for (int i = 0; i < meet->getSchools().length(); i++) {
        schoolSelector.addItem(meet->getSchools()[i]->getName());
    }
    lowerThirdButton.setText("Show L3rd");

    widgetLayout->addWidget(&laneNumber);
    widgetLayout->addWidget(&schoolSelector);
    widgetLayout->addWidget(&nameField);
    widgetLayout->addWidget(&lowerThirdButton);

    connect(&lowerThirdButton, SIGNAL(clicked(bool)), this, SLOT(prepareLt()));
    connect(this, SIGNAL(showLt(Swimmer,QString)), game->getLt(), SLOT(prepareSticker(Swimmer,QString)));

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

void ParticipantUI::prepareLt()
{
    Swimmer swimmer(nameField.text(), meet->getSchools()[schoolSelector.currentIndex()],laneNumber.text());
    emit showLt(swimmer, eventName);
}

void ParticipantUI::updateEventName(QString name)
{
    this->eventName=name;
}
