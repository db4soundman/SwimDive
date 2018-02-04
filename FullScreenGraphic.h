#ifndef FULLSCREENGRAPHIC_H
#define FULLSCREENGRAPHIC_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QList>
#include <QPainter>
#include <QPixmap>
#include <QLinearGradient>
#include "Swimmer.h"

class FullScreenGraphic: public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    FullScrenGraphic();



    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option, QWidget * widget = 0);



public slots:
    void hide();
    void showResultsWithTime(QList<Swimmer> presults, QString pevent);
    void showResults(QList<Swimmer> presults, QString pevent);
    void showLaneAssignments(QList<Swimmer> presults,QString pevent);

private:

    QLinearGradient mainGradient, subtitleGradient;
    QPixmap macLogo;
    bool show;
    QList<Swimmer> resultData;
    QString eventName;
    bool laneAssignments, results, showTimes;
};

#endif // FULLSCREENGRAPHIC_H
