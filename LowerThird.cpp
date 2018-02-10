#include "LowerThird.h"
#include <QFontMetrics>
#include <QStringRef>
#include <QGraphicsScene>
#include "GraphicChooser.txt"
#include "MiamiAllAccessHockey.h"

#define NAME_GRADIENT_LEVEL .3
#define STAT_GRADIENT_LEVEL .3
#define NAME_WIDTH 300
#define BOX_HEIGHT 38
LowerThird::LowerThird(int screenWidth, QGraphicsItem* parent) :
    name(""), number("number"), statFont("Arial", 22, QFont::Bold), nameFont("Arial", 28, QFont::Bold), labelFont("Arial", 18, QFont::Bold)
    {
#ifdef Q_OS_OSX
    statFont.setPointSize(36);
    nameFont.setPointSize(36);
    #endif
    mac = MiamiAllAccessHockey::getSwatchFromESPN("MAC");
    fontPointSize = nameFont.pointSize();
    //setPixmap(QPixmap(":/images/LowerThird.png"));
    setRect(0,0,1000, BOX_HEIGHT*2);
    statFontPointSize = statFont.pointSize();
    gradient.setStart(0, 0);
    gradient.setFinalStop(0, BOX_HEIGHT*3);
    homeNameGradient.setStart(0, 0);
    homeNameGradient.setFinalStop(0, BOX_HEIGHT*2);
    awayNameGradient.setStart(0, 0);
    awayNameGradient.setFinalStop(0, BOX_HEIGHT*2);
    statGradient.setStart(0, 0);
    statGradient.setFinalStop(0, BOX_HEIGHT * 3);
    homeStatGradient.setStart(0, BOX_HEIGHT);
    homeStatGradient.setFinalStop(0, BOX_HEIGHT * 2);
    awayStatGradient.setStart(0, 0);
    awayStatGradient.setFinalStop(0, BOX_HEIGHT);
    prepareColors();
    statistics.append("");
    statNames.append("");
    this->centerPoint = screenWidth / 2;
    show = false;
}

void
LowerThird::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                  QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (show) {

        painter->fillRect(0, 0, 1000, BOX_HEIGHT*2, statGradient);


        painter->setFont(nameFont);
        painter->setPen(QColor(255,255,255));
        painter->fillRect(0,0,NAME_WIDTH, BOX_HEIGHT*2, gradient);
        painter->drawText(60, 0, NAME_WIDTH, BOX_HEIGHT, Qt::AlignVCenter, name);
        //painter->drawText(60, BOX_HEIGHT, NAME_WIDTH, BOX_HEIGHT, Qt::AlignVCenter, lastName);
        painter->setFont(statFont);
        painter->drawText(NAME_WIDTH,0, rect().width() - NAME_WIDTH, BOX_HEIGHT, Qt::AlignCenter, eventName);
    }
}

void LowerThird::paintPreview(QPainter *painter)
{
    painter->fillRect(0, 0, 1000, BOX_HEIGHT*2, statGradient);


    painter->setFont(nameFont);
    painter->setPen(QColor(255,255,255));
    painter->fillRect(0,0,NAME_WIDTH, BOX_HEIGHT*2, gradient);
    painter->drawText(60, 0, NAME_WIDTH, BOX_HEIGHT, Qt::AlignVCenter, name);
    //painter->drawText(60, BOX_HEIGHT, NAME_WIDTH, BOX_HEIGHT, Qt::AlignVCenter, lastName);
    painter->setFont(statFont);
    painter->drawText(NAME_WIDTH,0, rect().width() - NAME_WIDTH, BOX_HEIGHT, Qt::AlignCenter, eventName);
//    painter->drawText(0, 0, 60, BOX_HEIGHT, Qt::AlignCenter, number);
//    painter->drawText(0, BOX_HEIGHT, 60, BOX_HEIGHT, Qt::AlignCenter, year);


//    int rectWidth = (1000 - NAME_WIDTH)/statistics.size();

//    painter->setPen(QColor(255, 255, 255));
//    // Stat numbers
//    for (int i = 0; i< statistics.size(); i++) {
//        painter->setFont(statFont);
//        painter->drawText(rectWidth * i + NAME_WIDTH, BOX_HEIGHT, rectWidth, BOX_HEIGHT, Qt::AlignHCenter | Qt::AlignBottom, statistics.at(i));
//        painter->setFont(labelFont);
//        painter->drawText(rectWidth * i + NAME_WIDTH, 0, rectWidth, BOX_HEIGHT, Qt::AlignHCenter | Qt::AlignTop, statNames.size() > i ? statNames.at(i) : " ");
//    }
}

int LowerThird::getWidth()
{
    return rect().width();
}

int LowerThird::getHeight()
{
    return rect().height();
}

void
LowerThird::prepareForDisplay(QString name, QString number, QString year,
                              QList<QString> statLabels,
                              QList<QString> statValues, bool homeTeam, bool goLive) {
//    this->name = name;
//    if (name.contains(" ")) {
//        firstName = name.left(name.indexOf(" "));
//        QStringRef substr(&name, name.indexOf(" ") + 1, name.length() - (name.indexOf(" ")+1));
//        lastName = substr.toString();
//    }
//    else {
//        firstName = name;
//        lastName = "";
//    }
//    this->year = year;
//    this->number = number;
//    statNames = statLabels;
//    statistics = statValues;
//    //gradient = homeTeam ? homeNameGradient : awayNameGradient;
//   // statGradient = homeTeam ? homeStatGradient : awayStatGradient;
//    prepareFontSize();
//    // To ensure font size is returned to normal in the event that
//    // a custom text LT was used.
//    statFont.setPointSize(statFontPointSize);
//    if (goLive) showLt();
}

void LowerThird::prepareForCustomLt(QString name, QString number, QString year,
                                    QList<QString> statLabels,
                                    QList<QString> statValues, bool homeTeam, bool goLive)
{
    statFont.setPointSize(statFontPointSize);
    this->name = name;
//    if (name.contains(" ")) {
//        firstName = name.left(name.indexOf(" "));
//        QStringRef substr(&name, name.indexOf(" ") + 1, name.length() - (name.indexOf(" ")+1));
//        lastName = substr.toString();
//    }
//    else {
//        firstName = name;
//        lastName = "";
//    }
    this->year = year;
    this->number = number;
    statNames = statLabels;
    statistics = statValues;
    //gradient = homeTeam ? homeNameGradient : awayNameGradient;
    //statGradient = homeTeam ? homeStatGradient : awayStatGradient;
    // Resize the font to be two lines ONLY if necessary...
    QFontMetrics fontSize(statFont);
    if (fontSize.width(statistics[0]) > this->rect().width() - NAME_WIDTH)
        adjustFont();
    if (goLive) showLt();
}

void LowerThird::prepareSticker(Swimmer swimmer, QString eventName)
{
    this->swimmer = swimmer;
    this->eventName = eventName;
    regenNameGradient(swimmer.getSchool()->getPrimaryColor());
    showLt();
}
void LowerThird::prepareColors() {
    int red, green, blue;
    QColor end;
    stats = mac.getPrimaryColor();
    red = -1*stats.red() *STAT_GRADIENT_LEVEL + stats.red();
    green = -1*stats.green() *STAT_GRADIENT_LEVEL + stats.green();
    blue = -1*stats.blue() *STAT_GRADIENT_LEVEL + stats.blue();
    end.setRgb(red, green, blue);
    if (end == QColor(0,0,0))
        end = QColor(1,1,1);
    statGradient.setColorAt(1, end);
    statGradient.setColorAt(0,stats);
}

void
LowerThird::prepareFontSize() {
    int subtraction = 1;
    QFontMetrics fontSize(nameFont);
    while (fontSize.width(name) > NAME_WIDTH) {
        QFont tempFont("Arial", fontPointSize - subtraction, QFont::Bold);
        //nameFont.setPointSize(fontPointSize - subtraction);
        subtraction++;
        nameFont = tempFont;
        QFontMetrics temp(nameFont);
        fontSize = temp;
    }
}

void LowerThird::adjustFont()
{
    statFont.setPointSize(22);
#ifdef Q_OS_OSX
    statFont.setPointSize(28);
#endif
    int subtraction = 1;
    QFontMetrics fontSize(statFont);
    while (fontSize.width(statistics[0]) > this->rect().width() - NAME_WIDTH) {
        QFont tempFont("Arial", statFont.pointSize() - subtraction, QFont::Bold);
        subtraction++;
        statFont = tempFont;
        QFontMetrics temp(statFont);
        fontSize = temp;
    }
}

void LowerThird::regenNameGradient(QColor c)
{
    int red, green, blue;
    QColor end;
    red = -1*c.red() *STAT_GRADIENT_LEVEL + c.red();
    green = -1*c.green() *STAT_GRADIENT_LEVEL + c.green();
    blue = -1*c.blue() *STAT_GRADIENT_LEVEL + c.blue();
    end.setRgb(red, green, blue);
    if (end == QColor(0,0,0))
        end = QColor(1,1,1);
    gradient.setColorAt(1, end);
    gradient.setColorAt(0,c);
}



void
LowerThird::hideLt() {
    if (show) {
        show = false;
        emit removeNoTransparencyZone(QRect(x() + 0 ,y() + 0,NAME_WIDTH, BOX_HEIGHT*2));
        scene()->update();
    }
}

void
LowerThird::showLt() {
    show = true;
    emit addNoTransparencyZone(QRect(x() + 0 ,y() + 0,NAME_WIDTH, BOX_HEIGHT*2));
    scene()->update();
}

void LowerThird::showPpComp()
{
    show = false;
    scene()->update();
}
