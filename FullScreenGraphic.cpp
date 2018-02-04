#include "FullScreenGraphic.h"
#define WIDTH 1920/2
#define HEIGHT 1080
#define TITLE_HEIGHT 50
#define SUBTITLE_HEIGHT 30
#define SWIMMER_START (TITLE_HEIGHT + SUBTITLE_HEIGHT)

FullScreenGraphic::FullScreenGraphic()
{
    setRect(0,0,WIDTH,HEIGHT);
    show = false;
    //macLogo = new QPixmap(":/images/NCHClrg.png");
    //*macLogo = nchcLogo->scaledToHeight(BORDER_HEIGHT, Qt::SmoothTransformation);
    mainGradient.setStart(0,0);
    mainGradient.setFinalStop(0,HEIGHT);
    mainGradient.setColorAt(0, QColor(41, 70, 91));
    mainGradient.setColorAt(1, QColor(41, 70, 91));

    laneAssignments=false;
    results = false;
    showTimes = false;
}

void FullScreenGraphic::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (show) {
        int fontSize = 20;
        QFont font("Arial", fontSize, QFont::Bold);
        font.setCapitalization(QFont::SmallCaps);
        painter->setFont(font);

        painter->setPen(QColor(255,255,255));
        painter->fillRect(rect(),mainGradient);
        painter->drawPixmap(0,0,macLogo);
        painter->drawText(50,0,WIDTH-50,40,Qt::AlignCenter,eventName);
        painter->fillRect(0,50,WIDTH,30,subtitleGradient);
        painter->drawText(0,50,WIDTH,30,Qt::AlignCenter,laneAssignments ?
                              "lane assignments" : "unofficial results");
        // Column headrs
        painter->drawText(0,SWIMMER_START,WIDTH/4,30, Qt::AlignCenter, "LANE");
        painter->drawText(SWIMMER_START,WIDTH/4,SWIMMER_START,WIDTH/4,30,
                          Qt::AlignCenter, "PARTICIPANT");
        painter->drawText(2*SWIMMER_START,WIDTH/4,SWIMMER_START,WIDTH/4,30, Qt::AlignCenter, "PLACE");
        if (showTimes)
            painter->drawText(3*SWIMMER_START,WIDTH/4,SWIMMER_START,WIDTH/4,30, Qt::AlignCenter, "TIME");

        for (int i = 0; i < resultData.size(); i++) {
            painter->drawText(0,SWIMMER_START,WIDTH/4,30, Qt::AlignCenter, resultData[i].getLaneNumber());
            painter->drawPixmap(20,SWIMMER_START, resultData[i].getSchool()->getLogo());
            painter->drawText(SWIMMER_START,WIDTH/4,SWIMMER_START,WIDTH/4,30,
                              Qt::AlignCenter, resultData[i].getName());
            painter->drawText(2*SWIMMER_START,WIDTH/4,SWIMMER_START,WIDTH/4,30, Qt::AlignCenter, resultData[i].getPlace());
            if (showTimes)
                painter->drawText(3*SWIMMER_START,WIDTH/4,SWIMMER_START,WIDTH/4,30, Qt::AlignCenter, resultData[i].getTime());
            // TODO Change color
            painter->drawLine(0,SWIMMER_START + 30, WIDTH,SWIMMER_START + 30);
        }
    }
}

void FullScreenGraphic::showImg()
{

}

void FullScreenGraphic::hide()
{

}

void FullScreenGraphic::showResultsWithTime(QList<Swimmer> presults, QString pevent)
{
    laneAssignments=false;
    results = true;
    showTimes = true;
    resultData = presults;
}

void FullScreenGraphic::showResults(QList<Swimmer> presults,QString pevent)
{
    laneAssignments=false;
    results = true;
    showTimes = false;
    resultData = presults;
}

void FullScreenGraphic::showLaneAssignments(QList<Swimmer> presults,QString pevent)
{
    laneAssignments=true;
    results = false;
    showTimes = false;
    resultData = presults;
}
