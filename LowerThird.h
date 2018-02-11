#ifndef LOWERTHIRD_H
#define LOWERTHIRD_H

#include <QGraphicsRectItem>
#include <QLinearGradient>
#include <QPainter>
#include <QObject>
#include <QList>
#include <QString>
#include <QFont>
#include <QColor>
#include <QRect>
#include "GraphicChooser.txt"

#include "Swimmer.h"
#include "School.h"
#include "Diver.h"

class LowerThird : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    LowerThird(int screenWidth, QGraphicsItem* parent = 0);

    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

    void paintPreview(QPainter* painter);

    int getWidth();
    int getHeight();


signals:
    void addNoTransparencyZone(QRect r);
    void removeNoTransparencyZone(QRect r);

public slots:
    void hideLt();
    void showLt();
    void showPpComp();
    void prepareForDisplay(QString name, QString number, QString year, QList<QString> statLabels,
                           QList<QString> statValues, bool homeTeam, bool goLive=true);
    void prepareForCustomLt(QString name, QString number, QString year, QList<QString> statLabels,
                           QList<QString> statValues, bool homeTeam,bool goLive=true);
    void prepareSticker(Swimmer swimmer, QString eventName);
    void prepareDiver(Diver d, QString eventName);
private:
    QList<QString> statNames;
    QList<QString> statistics;
    QString name, number, year;
    QFont statFont, nameFont, labelFont;
    QLinearGradient gradient, statGradient, totalGradient, homeNameGradient, awayNameGradient, homeStatGradient, awayStatGradient;
    QColor stats;

    QString homeName, awayName, homeLabel, awayLabel, eventName;

    School mac;
    Swimmer swimmer;
    Diver diver;

    void prepareColors();
    void prepareFontSize();
    void adjustFont();

    void regenNameGradient(QColor c);

    bool show, swimming;
    int fontPointSize, statFontPointSize, centerPoint, displayWidth;

    void getDisplayWidthSwimmer();
    void getDisplayWidthDiver();
};

#endif // LOWERTHIRD_H
