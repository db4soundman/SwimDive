#include "FullScreenGraphic.h"
#define WIDTH 1920*.75
#define HEIGHT 800
#define TITLE_HEIGHT 50
#define SUBTITLE_HEIGHT 30
#define HEADER_START (TITLE_HEIGHT + SUBTITLE_HEIGHT)
#define SWIMMER_START (HEADER_START + 30)

#define ROW_HEIGHT 80
#define TEXT_X (40 + largestWidth + 10)
#define GRADIENT_LEVEL .5

#define TEAM_GRADIENT_LEVEL .4
#define SCOREBOARD_GRADIENT_LEVEL .9

#include <QGraphicsScene>
#include "MiamiAllAccessHockey.h"
#include <QStandardPaths>

#include <algorithm>

FullScreenGraphic::FullScreenGraphic(QGraphicsRectItem* parent):
    QGraphicsRectItem(parent)
{
    setRect(0,0,WIDTH,HEIGHT);
    mac = MiamiAllAccessHockey::getSwatchFromESPN("MAC");
    show = false;

    prepareColor();

    laneAssignments=false;
    results = false;
    showTimes = false;
    largestWidth = 0;
}

void FullScreenGraphic::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (show) {
        int fontSize = 40;
        QFont font("Arial", fontSize, QFont::Bold);
        font.setCapitalization(QFont::SmallCaps);
        painter->setFont(font);

        painter->setPen(QColor(255,255,255));
        painter->fillRect(rect(),mainGradient);
        painter->fillRect(0,0,WIDTH,TITLE_HEIGHT,titleGradient);
        painter->drawPixmap(0,0,mac.getLogo().scaledToHeight(50, Qt::SmoothTransformation));
        painter->drawText(0,0,WIDTH,TITLE_HEIGHT,Qt::AlignCenter,eventName);
        painter->fillRect(0,TITLE_HEIGHT,WIDTH,30,subtitleGradient);
        font.setPointSize(24);
        painter->setFont(font);
        painter->drawText(0,TITLE_HEIGHT,WIDTH,30,Qt::AlignCenter,laneAssignments ?
                              "LANE ASSIGNMENTS" : "UNOFFICIAL RESULTS BY LANE");
        // Column headrs
//        if (!laneAssignments) {
//            painter->drawText(0,HEADER_START,WIDTH/4,30, Qt::AlignCenter, "LANE");
//            painter->drawText(WIDTH/4,HEADER_START,WIDTH/4,30,
//                              Qt::AlignCenter, "PARTICIPANT");
//            if (results)
//                painter->drawText(2*WIDTH/4,HEADER_START,WIDTH/4,30, Qt::AlignCenter, "PLACE");
//            if (showTimes)
//                painter->drawText(WIDTH/4,3*HEADER_START,WIDTH/4,30, Qt::AlignCenter, "TIME");
//        }
        QFont font2("Arial", 30, QFont::Bold);
        painter->setFont(font2);
        for (int i = 0; i < resultData.size(); i++) {
            painter->setPen(QColor(255,255,255));
            if (laneAssignments) painter->drawText(0,SWIMMER_START + ROW_HEIGHT*i,40,ROW_HEIGHT, Qt::AlignCenter, resultData[i].getLaneNumber());
            else painter->drawText(0,SWIMMER_START + ROW_HEIGHT*i,40,ROW_HEIGHT, Qt::AlignCenter, resultData[i].getPlace());
            painter->fillRect(40,SWIMMER_START+ ROW_HEIGHT*i,largestWidth,ROW_HEIGHT,resultData[i].getSchool()->getPrimaryColor());
            QPixmap logo = resultData[i].getSchool()->getLogo().scaledToHeight(ROW_HEIGHT-10,Qt::SmoothTransformation);
            painter->drawPixmap(40 + ((largestWidth - logo.width()) / 2),SWIMMER_START+ ROW_HEIGHT*i +5, logo);
            painter->drawText(TEXT_X,SWIMMER_START + ROW_HEIGHT*i,WIDTH/2,ROW_HEIGHT,
                              Qt::AlignVCenter, resultData[i].getName());
            if (showTimes)
                painter->drawText(3*WIDTH/4,SWIMMER_START+ ROW_HEIGHT*i,WIDTH/4,ROW_HEIGHT, Qt::AlignCenter, resultData[i].getTime());


        }
        for (int i = 0; i < resultData.size(); i++) {
            painter->setPen(QColor(100,100,100));
            painter->drawLine(0,SWIMMER_START + ROW_HEIGHT + ROW_HEIGHT*i, WIDTH,SWIMMER_START + ROW_HEIGHT+ ROW_HEIGHT*i);
        }
    }
}

void FullScreenGraphic::hide()
{
    show = false;
    scene()->update(x(), y(), WIDTH, HEIGHT);
}

void FullScreenGraphic::showResultsWithTime(QList<Swimmer> presults, QString pevent)
{
    laneAssignments=false;
    results = true;
    showTimes = true;
    resultData = presults;
    eventName=pevent;
    show=true;
    calculateLargestWidth();
    scene()->update(x(), y(), WIDTH, HEIGHT);
}

void FullScreenGraphic::showResults(QList<Swimmer> presults,QString pevent)
{
    laneAssignments=false;
    results = true;
    showTimes = false;
    resultData = presults;
    eventName=pevent;
    show=true;
    calculateLargestWidth();
    scene()->update(x(), y(), WIDTH, HEIGHT);
}

void FullScreenGraphic::showLaneAssignments(QList<Swimmer> presults,QString pevent)
{
    laneAssignments=true;
    results = false;
    showTimes = false;
    resultData = presults;
    eventName=pevent;
    show=true;
    calculateLargestWidth();
    scene()->update(x(), y(), WIDTH, HEIGHT);
}

void FullScreenGraphic::prepareColor()
{
    mainGradient.setStart(0,0);
    mainGradient.setFinalStop(0,HEIGHT);

    titleGradient.setStart(0,0);
    titleGradient.setFinalStop(0,TITLE_HEIGHT);

    subtitleGradient.setStart(0,TITLE_HEIGHT);
    subtitleGradient.setFinalStop(0,TITLE_HEIGHT + SUBTITLE_HEIGHT);

    int red, green, blue;
    red = -1*mac.getSecondaryColor().red() *TEAM_GRADIENT_LEVEL + mac.getSecondaryColor().red();
    green = -1*mac.getSecondaryColor().green() *TEAM_GRADIENT_LEVEL + mac.getSecondaryColor().green();
    blue = -1*mac.getSecondaryColor().blue() *TEAM_GRADIENT_LEVEL + mac.getSecondaryColor().blue();

    QColor end(red, green, blue);
    if (end == QColor(0,0,0))
        end = QColor(1,1,1);
    titleGradient.setColorAt(0, mac.getSecondaryColor());
    titleGradient.setColorAt(1, end);

    QColor lightBlue(196, 213, 242);

    red = -1*lightBlue.red() *TEAM_GRADIENT_LEVEL + lightBlue.red();
    green = -1*lightBlue.green() *TEAM_GRADIENT_LEVEL + lightBlue.green();
    blue = -1*lightBlue.blue() *TEAM_GRADIENT_LEVEL + lightBlue.blue();

    end.setRed(red);
    end.setGreen(green);
    end.setBlue(blue);
    if (end == QColor(0,0,0))
        end = QColor(1,1,1);
    subtitleGradient.setColorAt(0, lightBlue);
    subtitleGradient.setColorAt(1, end);

    red = -1*mac.getPrimaryColor().red() *TEAM_GRADIENT_LEVEL + mac.getPrimaryColor().red();
    green = -1*mac.getPrimaryColor().green() *TEAM_GRADIENT_LEVEL + mac.getPrimaryColor().green();
    blue = -1*mac.getPrimaryColor().blue() *TEAM_GRADIENT_LEVEL + mac.getPrimaryColor().blue();

    end.setRed(red);
    end.setGreen(green);
    end.setBlue(blue);
    if (end == QColor(0,0,0))
        end = QColor(1,1,1);
    mainGradient.setColorAt(0, mac.getPrimaryColor());
    mainGradient.setColorAt(1, end);
}

void FullScreenGraphic::calculateLargestWidth()
{
    QList<int> widths;
    for (int i = 0; i < resultData.size(); i++) {
        widths.append(resultData[i].getSchool()->getLogo().scaledToHeight(ROW_HEIGHT-10).width());
    }

    std::sort(widths.begin(), widths.end(), std::greater<int>());
    largestWidth = widths[0];
}
