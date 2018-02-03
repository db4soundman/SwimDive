#ifndef FULLSCREENGRAPHIC_H
#define FULLSCREENGRAPHIC_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QList>
#include <QPainter>
#include <QPixmap>
#include <QLinearGradient>

class FullScreenGraphic: public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    FullScrenGraphic();



    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option, QWidget * widget = 0);



public slots:
    void showImg();
    void hide();

private:

    QLinearGradient mainGradient;
    QPixmap* nchcLogo;
    bool show;
    QString leftHeader, rightHeader;
};

#endif // FULLSCREENGRAPHIC_H
