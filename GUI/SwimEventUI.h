#ifndef SWIMEVENTUI_H
#define SWIMEVENTUI_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>

#include "SwimMeet.h"
#include "ParticipantUI.h"

class SwimEventUI : public QWidget
{
    Q_OBJECT
public:
    explicit SwimEventUI(SwimMeet* meet, QWidget *parent = nullptr);

signals:

public slots:

private:
    SwimMeet* meet;
    QList<ParticipantUI*> participantUIs;
    QPushButton laneAssignments, finalResults, enterFinalResults;
    QList<QString> times;

};

#endif // SWIMEVENTUI_H
