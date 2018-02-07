#include "ParticipantUI.h"

#include <QHBoxLayout>

ParticipantUI::ParticipantUI(SwimMeet *meet, QString laneNo, QWidget *parent) : QWidget(parent)
{
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
