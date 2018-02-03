#include "SwimEventUI.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
SwimEventUI::SwimEventUI(SwimMeet *meet, QWidget *parent) : QWidget(parent)
{
    laneAssignments.setText("Show Lanes");
    finalResults.setText("Show Final Results");
    enterFinalResults.setText("Input Results");

    QHBoxLayout* buttons = new QHBoxLayout();
    buttons->addWidget(&laneAssignments);
    buttons->addWidget(&finalResults);
    buttons->addWidget(&enterFinalResults);

    QVBoxLayout* widgetLayout = new QVBoxLayout();
    widgetLayout->addLayout(buttons);

    for (int i = 0; i < 8; i++) {
        participantUIs.append(new ParticipantUI(meet));
    }
    for (int i = 0; i < 8; i++) {
        widgetLayout->addWidget(participantUIs[i]);

    }



    setLayout(widgetLayout);
}

