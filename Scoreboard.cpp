#include "Scoreboard.h"
#include <QGraphicsScene>
#include <QMessageBox>
#include <QFontMetrics>
#include <QFontInfo>
#include "GraphicChooser.txt"
#include "MiamiAllAccessHockey.h"

// Flat look
#define GRADIENT_LEVEL .5

#define TEAM_GRADIENT_LEVEL .4
#define SCOREBOARD_GRADIENT_LEVEL .9


#define AWAY_PP 1
#define HOME_PP 2
#define NEUTRAL 3
#define TEAM_NAME_WIDTH 310
#define SCOREBOARD_HEIGHT 44
#define SCORE_WIDTH 78
#define TEAM_WIDTH (TEAM_NAME_WIDTH + SCORE_WIDTH)
#define RANK_WIDTH 30
#define TEAM_BOX_Y 0
#define TEAM_BOX_HEIGHT 44
#define V_TEAM_BOX_STARTX 180
#define H_TEAM_BOX_STARTX (V_TEAM_BOX_STARTX + TEAM_WIDTH + 5)
#define LOGO_WIDTH 50
#define SCOREBOARD_WIDTH 300
#define CLOCK_FIELD_X 150
#define CLOCK_FIELD_WIDTH 150
#define PP_BAR_HEIGHT 30
#define TOP_BAR_WIDTH (SCOREBOARD_WIDTH - 40)
#define TOP_BAR_HEIGHT 0

Scoreboard::Scoreboard(QColor awayCol, QColor homeCol, QString awayTeam, QString homeTeam,
                       QString sponsorText, Clock* clock, QString pAwayRank, QString pHomeRank, QPixmap pawayLogo) :
    macPrimaryColor(homeCol), macSecondaryColor(awayCol) {
    QFont font("Arial", 28, QFont::Bold);
    font.setCapitalization(QFont::SmallCaps);
    QFont sponsorFont("Arial", 18, QFont::Bold);
    sponsorFont.setCapitalization(QFont::SmallCaps);
#ifdef Q_OS_OSX
    font.setPointSize(40);
    sponsorFont.setPointSize(28);
#endif

    nchctv = (MiamiAllAccessHockey::getImgFromResources(":/images/NCHCTV.png",42));
    defaultSponsorText = sponsorFont;
    show = false;
    setRect(0,0,SCOREBOARD_WIDTH, TOP_BAR_HEIGHT + SCOREBOARD_HEIGHT + PP_BAR_HEIGHT);
    altAwayLogoBg = false;

    useClock = true;
    awayName = new QGraphicsTextItem(awayTeam);
    homeName = new QGraphicsTextItem(homeTeam);
    awayName->setFont(font);
    homeName->setFont(font);
    awayScore = new QGraphicsTextItem("0");
    awayScore->setFont(font);
    homeScore = new QGraphicsTextItem("0");
    homeScore->setFont(font);
    topBarText = new QGraphicsTextItem(sponsorText);
    this->sponsorText = sponsorText;
    topBarText->setFont(defaultSponsorText);

    macPrimaryGradient.setStart(0, TOP_BAR_HEIGHT + TEAM_BOX_Y);
    macSecondaryGradient.setStart(0, TOP_BAR_HEIGHT + TEAM_BOX_HEIGHT);
    macPrimaryGradient.setFinalStop(0,TOP_BAR_HEIGHT + TEAM_BOX_HEIGHT);
    macSecondaryGradient.setFinalStop(0,TOP_BAR_HEIGHT + TEAM_BOX_HEIGHT + PP_BAR_HEIGHT);
    prepareColor();

    sponsor = true;
    showPdAndClockFields = true;
    showClock = true;
    useTransparency = false;

    this->clock = clock;
    connect(clock, SIGNAL(clockUpdated()), this, SLOT(updateClock()));
}


void
Scoreboard::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                  QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (show) {
painter->fillRect(0,TOP_BAR_HEIGHT + TEAM_BOX_HEIGHT,SCOREBOARD_WIDTH, PP_BAR_HEIGHT,macSecondaryGradient);
        painter->fillRect(0,TOP_BAR_HEIGHT,SCOREBOARD_WIDTH, SCOREBOARD_HEIGHT,macPrimaryGradient);

        //Clock - Game time...draw clock first since default color is black
        painter->setPen(QColor(230,230,230));
        //painter->fillRect(CLOCK_FIELD_X, 1 + TOP_BAR_HEIGHT, CLOCK_FIELD_WIDTH,SCOREBOARD_HEIGHT-2, clockGradient);
        painter->setFont(defaultSponsorText);
       painter->drawText(3, TOP_BAR_HEIGHT, CLOCK_FIELD_WIDTH, SCOREBOARD_HEIGHT, Qt::AlignVCenter, period);
       painter->setFont(homeName->font());
         painter->drawText(CLOCK_FIELD_X, TOP_BAR_HEIGHT, CLOCK_FIELD_WIDTH-10, SCOREBOARD_HEIGHT, Qt::AlignRight | Qt::AlignVCenter,clock->toString() );


        painter->setFont(topBarText->font());
        painter->drawText(0,TEAM_BOX_HEIGHT, SCOREBOARD_WIDTH, PP_BAR_HEIGHT, Qt::AlignCenter, topBarText->toPlainText().toLower());
        // Away logo

        painter->setPen(QColor(196, 213, 242));
        painter->drawRect(0,TOP_BAR_HEIGHT,SCOREBOARD_WIDTH, SCOREBOARD_HEIGHT);
        painter->setPen(QColor(255, 255, 255));

    }
}

void
Scoreboard::prepareColor() {
    int red, green, blue;
    red = -1*macPrimaryColor.red() *TEAM_GRADIENT_LEVEL + macPrimaryColor.red();
    green = -1*macPrimaryColor.green() *TEAM_GRADIENT_LEVEL + macPrimaryColor.green();
    blue = -1*macPrimaryColor.blue() *TEAM_GRADIENT_LEVEL + macPrimaryColor.blue();

    QColor end(red, green, blue);
    if (end == QColor(0,0,0))
        end = QColor(1,1,1);
    macPrimaryGradient.setColorAt(0, macPrimaryColor);
   // homeGradient.setColorAt(.6, homeColor);
    macPrimaryGradient.setColorAt(1, end);
    //homeGradient.setColorAt(0, end);


    red = -1*macSecondaryColor.red() *TEAM_GRADIENT_LEVEL + macSecondaryColor.red();
    green = -1*macSecondaryColor.green() *TEAM_GRADIENT_LEVEL + macSecondaryColor.green();
    blue = -1*macSecondaryColor.blue() *TEAM_GRADIENT_LEVEL + macSecondaryColor.blue();
    QColor end2(red, green, blue);
    if (end2 == QColor(0,0,0))
        end2 = QColor(1,1,1);
    macSecondaryGradient.setColorAt(0, macSecondaryColor);
    //awayGradient.setColorAt(.6, awayColor);
    macSecondaryGradient.setColorAt(1, end2);
    //awayGradient.setColorAt(0, end2);

}

void Scoreboard::prepareAwayName()
{
    int subtraction = 1;
    int fontPointSize = awayName->font().pointSize();
    QFontMetrics fontSize(awayName->font());
    while (fontSize.width(awayName->toPlainText()) > TEAM_NAME_WIDTH - awayRankOffset) {
        QFont tempFont("Arial", fontPointSize - subtraction, QFont::Bold);
        //topBarText->font().setPointSize(defaultSponsorText.pointSize()-subtraction);
        subtraction++;
        awayName->setFont(tempFont);
        QFontMetrics temp(awayName->font());
        fontSize = temp;
    }
}

void
Scoreboard::togglePenalty() {
    penalty = !penalty;
    scene()->update();
}

void
Scoreboard::updateClock() {
    scene()->update(this->x() + CLOCK_FIELD_X, this->y() + TOP_BAR_HEIGHT + 3, CLOCK_FIELD_WIDTH, TEAM_BOX_HEIGHT);
}

void
Scoreboard::preparePowerplayClock(int pos, Clock *clock, QString description) {
    // Clear off current board.
    awayPP = false;
    homePP = false;
    neutralPP = false;
    ppClock = clock;
    if (ppClock != NULL) {
        switch (pos) {
        case AWAY_PP:
            awayPP = true;
            break;
        case HOME_PP:
            homePP = true;
            break;
        case NEUTRAL:
            neutralPP = true;
            break;
        default:
            break;
        }
    }
    ppDescription = description;
    scene()->update(this->x(), this->y() + TOP_BAR_HEIGHT + SCOREBOARD_HEIGHT,
                    SCOREBOARD_WIDTH + 4, PP_BAR_HEIGHT+1);
}

void
Scoreboard::updateAwayScore(int score) {
    QString scoreText;
    awayScore->setPlainText(scoreText.setNum(score, 10));
    scene()->update();
}

void
Scoreboard::updateHomeScore(int score) {
    QString str;
    homeScore->setPlainText(str.setNum(score, 10));
    scene()->update();
}

void
Scoreboard::updatePeriod(int pd) {
    this->pd=pd;
    switch (pd) {
    case 1:
        period = "1st";
        showPdAndClockFields = true;
        if (!useClock) {
            centeredTimeText = period + " PD";
        }
        break;
    case 2:
        period = "2nd";
        if (!useClock) {
            centeredTimeText = period + " PD";
        }
        break;
    case 3:
        period = "3rd";
        if (!useClock) {
            centeredTimeText = period + " PD";
        }
        break;
    case 4:
        period = "OT";
        showPdAndClockFields = true;
        if (!useClock) {
            centeredTimeText = "OVERTIME";
        }
        break;
    case 5:
        period = "3x3OT";
        showPdAndClockFields = true;
        if (!useClock) {
            centeredTimeText = "OVERTIME";
        }
        break;
    case 6:
        centeredTimeText = "SHOOTOUT";
        showPdAndClockFields = false;
        break;
    default:
        period = "";
        showPdAndClockFields = false;
        break;
    }
    scene()->update(x() + CLOCK_FIELD_X, y() + TOP_BAR_HEIGHT, CLOCK_FIELD_WIDTH, SCOREBOARD_HEIGHT);
}

void
Scoreboard::showPd() {
    showPdAndClockFields = true;
    scene()->update(x() + CLOCK_FIELD_X, y() + TOP_BAR_HEIGHT, CLOCK_FIELD_WIDTH, SCOREBOARD_HEIGHT);
}

void
Scoreboard::final() {
    showPdAndClockFields = false;
    centeredTimeText = "FINAL";
    scene()->update(x() + CLOCK_FIELD_X, y() + TOP_BAR_HEIGHT, CLOCK_FIELD_WIDTH, SCOREBOARD_HEIGHT);
}

void
Scoreboard::changeTopBarText(QString text) {
    topBarText->setPlainText(text);
    int subtraction = 1;
    QFont f("Arial", 20, QFont::Bold);
    f.setCapitalization(QFont::SmallCaps);
    topBarText->setFont(f);
    topBarText->font().setCapitalization(QFont::SmallCaps);
    QFontMetrics fontSize(topBarText->font());
    while (fontSize.width(text) > SCOREBOARD_WIDTH - 10) {
        QFont tempFont("Arial", defaultSponsorText.pointSize() - subtraction, QFont::Bold);
        tempFont.setCapitalization(QFont::SmallCaps);
        subtraction++;
        topBarText->setFont(tempFont);
        QFontMetrics temp(topBarText->font());
        fontSize = temp;
    }
    scene()->update(x(), y() + TEAM_BOX_HEIGHT,SCOREBOARD_WIDTH,PP_BAR_HEIGHT);
}

void
Scoreboard::displaySponsor() {
    topBarText->setPlainText(sponsorText);
    int subtraction = 1;
    topBarText->setFont(defaultSponsorText);
    QFontMetrics fontSize(topBarText->font());
    while (fontSize.width(sponsorText) > SCOREBOARD_WIDTH - 10) {
        QFont tempFont("Arial", defaultSponsorText.pointSize() - subtraction, QFont::Bold);
        tempFont.setCapitalization(QFont::SmallCaps);
        //topBarText->font().setPointSize(defaultSponsorText.pointSize()-subtraction);
        subtraction++;
        topBarText->setFont(tempFont);
        QFontMetrics temp(topBarText->font());
        fontSize = temp;
    }
 scene()->update(x(), y() + TEAM_BOX_HEIGHT,SCOREBOARD_WIDTH,PP_BAR_HEIGHT);
}


bool Scoreboard::getUseTransparency() const
{
    return useTransparency;
}

void Scoreboard::setUseTransparency(bool value)
{
    useTransparency = value;
}

int Scoreboard::getRealWidth()
{
    return SCOREBOARD_WIDTH;
}


void
Scoreboard::toggleShowBoard() {
    show = true;
    if (useTransparency)
        emit transparentField(x()+20,y(),TOP_BAR_WIDTH,TOP_BAR_HEIGHT);
    emit addNoTransparencyZone(QRect(x() + V_TEAM_BOX_STARTX, y() + TOP_BAR_HEIGHT + TEAM_BOX_Y, TEAM_WIDTH, TEAM_BOX_HEIGHT));
    scene()->update();
}

void Scoreboard::togglePpClocks()
{
    showPP = !showPP;
    scene()->update(this->x(), this->y() + TOP_BAR_HEIGHT + SCOREBOARD_HEIGHT,
                    SCOREBOARD_WIDTH + 4 , PP_BAR_HEIGHT+1);
}

void
Scoreboard::hideBoard() {
    if (show) {
        show = false;
        emit removeTransparentField(x()+20, y(), TOP_BAR_WIDTH,TOP_BAR_HEIGHT);
        emit removeNoTransparencyZone(QRect(x() + V_TEAM_BOX_STARTX, y() + TOP_BAR_HEIGHT + TEAM_BOX_Y, TEAM_WIDTH, TEAM_BOX_HEIGHT));
        scene()->update();
    }
}

void
Scoreboard::intermission() {
    if (useClock) {
    showPdAndClockFields = true;
    showClock = false;
    }
    else {
       centeredTimeText = period + " " + "INT";
    }
    scene()->update(x() + CLOCK_FIELD_X, y() + TOP_BAR_HEIGHT, CLOCK_FIELD_WIDTH, SCOREBOARD_HEIGHT);
}

void
Scoreboard::displayClock() {
    if (useClock) {
    showPdAndClockFields = true;
    }
    else {
        updatePeriod(pd);
    }
    showClock = true;
    scene()->update(x() + CLOCK_FIELD_X, y() + TOP_BAR_HEIGHT, CLOCK_FIELD_WIDTH, SCOREBOARD_HEIGHT);
}

void Scoreboard::changeUseClock(bool uc)
{
    useClock = uc;
    updatePeriod(pd);
}

void Scoreboard::toggleAwayLogoBg(bool on)
{
    altAwayLogoBg = on;
    if (show) {
        scene()->update(this->x(), this->y(), SCOREBOARD_WIDTH, SCOREBOARD_HEIGHT + TOP_BAR_HEIGHT);
    }
}

void Scoreboard::usingAllSport()
{
    serialPP = true;
}

void Scoreboard::usingInternalClocks()
{
    serialPP = false;
}

bool Scoreboard::getShowClock() const
{
    return showClock;
}

void Scoreboard::setSerialPowerPlay(int pos, QString clock, QString description)
{
    awayPP = false;
    homePP = false;
    neutralPP = false;
    if (!clock.isEmpty()) {
        switch (pos) {
        case AWAY_PP:
            awayPP = true;
            break;
        case HOME_PP:
            homePP = true;
            break;
        case NEUTRAL:
            neutralPP = true;
            break;
        default:
            break;
        }
    }
    ppDescription = description;
    serialPpClock = clock;
    scene()->update(this->x(), this->y() + TOP_BAR_HEIGHT + SCOREBOARD_HEIGHT,
                    SCOREBOARD_WIDTH + 4, PP_BAR_HEIGHT+1);

}
