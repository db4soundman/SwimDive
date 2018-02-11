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
#include "Diver.h"
class ParticipantUI : public QWidget
{
    Q_OBJECT
public:
    ParticipantUI(SwimMeet* meet, QString laneNo, HockeyGame* game, bool diving=false ,QString eventName="",QWidget *parent = nullptr);

    QString getName() const;

    School* getSchool() const;

    QString getLaneNumber() const;

    Diver getDiver();

signals:
    void showLt(Swimmer swimmer, QString eventName);
    void showDiverLt(Diver swimmer, QString eventName);

public slots:
    void prepareLt();
    void updateEventName(QString name);
    void addScore();
private:
    SwimMeet* meet;
    QLineEdit nameField;
    QPushButton lowerThirdButton, addScoreButton;
    QComboBox schoolSelector;
    QLabel laneNumber;
    QString eventName;
    Diver diver;
    bool diving;
};

#endif // PARTICIPANTUI_H
