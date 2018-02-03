#include "ParticipantUI.h"

#include <QHBoxLayout>

ParticipantUI::ParticipantUI(SwimMeet *meet, QWidget *parent) : QWidget(parent)
{
    QHBoxLayout* widgetLayout = new QHBoxLayout();
    laneNumber.setText("1");
    for (int i = 0; i < meet->getSchools().length(); i++) {
        schoolSelector.addItem(meet->getSchools()[i].getName());
    }
    lowerThirdButton.setText("Show L3rd");

    widgetLayout->addWidget(&laneNumber);
    widgetLayout->addWidget(&schoolSelector);
    widgetLayout->addWidget(&nameField);
    widgetLayout->addWidget(&lowerThirdButton);

    setLayout(widgetLayout);
}
