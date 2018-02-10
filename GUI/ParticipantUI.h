#ifndef PARTICIPANTUI_H
#define PARTICIPANTUI_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>

#include "SwimMeet.h"
#include "HockeyGame.h"
class ParticipantUI : public QWidget
{
    Q_OBJECT
public:
    ParticipantUI(SwimMeet* meet, QString laneNo, HockeyGame* game, QString eventName="",QWidget *parent = nullptr);

    QString getName() const;

    School* getSchool() const;

    QString getLaneNumber() const;

signals:
    void showLt(Swimmer swimmer, QString eventName);

public slots:
    void prepareLt();
    void updateEventName(QString name);
private:
    SwimMeet* meet;
    QLineEdit nameField;
    QPushButton lowerThirdButton;
    QComboBox schoolSelector;
    QLabel laneNumber;
    QString eventName;
};

#endif // PARTICIPANTUI_H
