#ifndef PARTICIPANTUI_H
#define PARTICIPANTUI_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>

#include "SwimMeet.h"
class ParticipantUI : public QWidget
{
    Q_OBJECT
public:
    ParticipantUI(SwimMeet* meet,QWidget *parent = nullptr);

signals:

public slots:

private:
    SwimMeet* meet;
    QLineEdit nameField;
    QPushButton lowerThirdButton;
    QComboBox schoolSelector;
    QLabel laneNumber;
};

#endif // PARTICIPANTUI_H
