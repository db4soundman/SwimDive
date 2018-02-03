#ifndef STATCREWSCANNER_H
#define STATCREWSCANNER_H

#include <QObject>
#include <QTimer>
#include "GameXmlUpdater.h"
#include "HockeyGame.h"
#include <QThread>
class StatCrewScanner : public QThread
{
    Q_OBJECT
public:
    StatCrewScanner(HockeyGame* game, QString fileName);

public slots:
    void toggleScanner(bool clockStatus);
    void toggleScanner(int pd);
    void toggleScanner();
    void run();
    void toggleEnabled();

signals:
    void statCrewStatus(bool on);

private slots:
    void updateStats();

private:
    GameXmlUpdater* statCrew;
    QTimer* inGame, *breakTime;
    bool isActive, enabled;
    QString statFile;
};

#endif // STATCREWSCANNER_H
