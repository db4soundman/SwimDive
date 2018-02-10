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
    FullScreenGraphic(QGraphicsRectItem* parent=0);



    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

signals:
    void addNoTransparencyZone(QRect r);
    void removeNoTransparencyZone(QRect r);


public slots:
    void hide();
    void showResultsWithTime(QList<Swimmer> presults, QString pevent);
    void showResults(QList<Swimmer> presults, QString pevent);
    void showLaneAssignments(QList<Swimmer> presults,QString pevent);

private:

    QLinearGradient mainGradient, subtitleGradient, titleGradient;
    School mac;
    bool show;
    QList<Swimmer> resultData;
    QString eventName;
    bool laneAssignments, results, showTimes;
    int largestWidth;


    void prepareColor();
    void calculateLargestWidth();
};

#endif // FULLSCREENGRAPHIC_H
