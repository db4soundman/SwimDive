#include "SetupPage.h"
#include <QGridLayout>
#include <QFileDialog>
#include <QColorDialog>
#include <QStandardPaths>
#include <QTextStream>
#include <QMessageBox>


SetupPage::SetupPage(QString* pAwayName, QString* pHomeName, QString* pAwayFile,
                     QString* pHomeFile, QString* pSponsor, QString* pAnnouncer,
                     QString* pAwayRank, QString* pHomeRank, QColor* pAwayColor,
                     QColor* pHomeColor, QColor* pBg, QString* pStatCrew,
                     bool *pUsingTricaster, QString* pawayLogo, QString* tricasterIp,
                     QString* aSname, QString* hSname, int *portNum): homeColorPrev(16,16),
    awayColorPrev(16,16) {
    swatchSelector = new QComboBox();
    swatchSelector->addItem("1");
    swatchSelector->addItem("2");
    homeColorPrev.fill(*pHomeColor);
    awayColorPrev.fill(*pAwayColor);
    homeColorBox = new QLabel();
    awayColorBox = new QLabel();
    homeColorBox->setPixmap(homeColorPrev);
    awayColorBox->setPixmap(awayColorPrev);
    browseAway.setText("Stat File");
    browseHome.setText("Stat File");
    chooseAColor.setText("Color");
    chooseHColor.setText("Color");
    chooseBg.setText("Background Color");
    browseStatCrew.setText("GameFile");
    browseLogo.setText("Logo");
    profileDialog.setText("Load Profile");
    ipHelp.setText("How do I determine this?");
    awayName = pAwayName;
    homeName = pHomeName;
    awayFile = pAwayFile;
    homeFile = (pHomeFile);
    sponsor = (pSponsor);
    announcer = (pAnnouncer);
    awayRank = (pAwayRank);
    homeRank = (pHomeRank);
    awayColor = (pAwayColor);
    homeColor = pHomeColor;
    statCrew = pStatCrew;
    awayShort = aSname;
    homeShort = hSname;
    bg = pBg;
    usingTricaster = pUsingTricaster;
    awayLogo = pawayLogo;
    this->tricasterIp = tricasterIp;
    port = portNum;
    QGridLayout* mainLayout = new QGridLayout();
    mainLayout->addWidget(new QLabel("Away Team:"), 0, 0);
    mainLayout->addWidget(&awayNameLine, 0, 1);
    mainLayout->addWidget(&chooseAColor, 0, 2);
    mainLayout->addWidget(awayColorBox,0,3);
    mainLayout->addWidget(new QLabel("Swatch Color:"), 0, 4);
    mainLayout->addWidget(swatchSelector, 0, 5);
    mainLayout->addWidget(&browseAway, 0, 6);
    mainLayout->addWidget(&browseLogo, 0, 7);
    mainLayout->addWidget(&profileDialog, 0,8);

    mainLayout->addWidget(new QLabel("Home Team:"), 1, 0);
    mainLayout->addWidget(&homeNameLine, 1, 1);
    mainLayout->addWidget(&chooseHColor, 1, 2);
    mainLayout->addWidget(homeColorBox,1,3);
    mainLayout->addWidget(&browseHome, 1, 4);

    mainLayout->addWidget(new QLabel("Away Team Rank:"), 2, 0);
    mainLayout->addWidget(&awayRankLine, 2, 1);
    mainLayout->addWidget(new QLabel("Away Team Short Name:"), 2, 2);
    mainLayout->addWidget(&awayShortLine, 2, 3,1,3);
    mainLayout->addWidget(new QLabel("Home Team Rank:"), 3, 0);
    mainLayout->addWidget(&homeRankLine, 3, 1);
    mainLayout->addWidget(new QLabel("Home Team Short Name:"), 3, 2);
    mainLayout->addWidget(&homeShortLine, 3, 3,1,3);
    mainLayout->addWidget(&chooseBg, 4, 0);
    sponsorLine.setText(*sponsor);
    mainLayout->addWidget(new QLabel("Sponsor:"), 5, 0);
    mainLayout->addWidget(&sponsorLine, 5, 1);
    mainLayout->addWidget(new QLabel("Announcers"), 6, 0);
    mainLayout->addWidget(&announcerLine, 6, 1);
    //mainLayout->addWidget(new QLabel("StatCrew File"), 7, 0);
    //mainLayout->addWidget(&browseStatCrew, 7, 1);
    tricasterBox = new QCheckBox("Using Tricaster");
    tricasterBox->setChecked(true);
    mainLayout->addWidget(tricasterBox, 8, 0);
    tricasterIpLine.setText(*tricasterIp);
    mainLayout->addWidget(&tricasterIpLine, 8, 1);
    mainLayout->addWidget(&ipHelp,8, 2);
    mainLayout->addWidget(new QLabel("Net Input"),8, 3);
    portSelector = new QComboBox();
    mainLayout->addWidget(portSelector,8, 4);

    portSelector->addItem("1");
    portSelector->addItem("2");

    setLayout(mainLayout);

    connect(&browseAway, SIGNAL(clicked()), this, SLOT(awayBrowse()));
    connect(&browseHome, SIGNAL(clicked()), this, SLOT(homeBrowse()));
    connect(&chooseAColor, SIGNAL(clicked()), this, SLOT(awayColorDiag()));
    connect(&chooseHColor, SIGNAL(clicked()), this, SLOT(homeColorDiag()));
    connect(&chooseBg, SIGNAL(clicked()), this, SLOT(bgDiag()));
    connect(&browseStatCrew, SIGNAL(clicked()), this, SLOT(statCrewBrowse()));
    connect(&browseLogo, SIGNAL(clicked()), this, SLOT(logoBrowse()));
    connect(&profileDialog, SIGNAL(clicked()), this, SLOT(profileBrowse()));
    connect(swatchSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(applyProfile()));
    connect(&ipHelp, SIGNAL(clicked()), this, SLOT(showHelp()));

    homeNameLine.setText(*homeName);
    announcerLine.setText(*announcer);
    homeShortLine.setText(*homeShort);
    setTitle("Game Information");
    //connect(&profileSelector, SIGNAL(closed(Profile)), this, SLOT(applyProfile(Profile)));
}

bool SetupPage::validatePage()
{
    *awayRank = awayRankLine.text();
    *homeRank = homeRankLine.text();
    *sponsor = sponsorLine.text();
    *announcer = announcerLine.text();
    *awayName = awayNameLine.text().toUpper();
    *homeName = homeNameLine.text().toUpper();
    *usingTricaster = tricasterBox->isChecked();
    *tricasterIp = tricasterIpLine.text();
    *awayShort = awayShortLine.text().toUpper();
    *homeShort = homeShortLine.text().toUpper();
    *port = portSelector->currentIndex() + 7000;
    return true;
}

void SetupPage::homeBrowse()
{
    QString file = QFileDialog::getOpenFileName(0, "Home File");
    if (!file.isEmpty())
        *homeFile = file;
}

void SetupPage::awayBrowse()
{
    QString file = QFileDialog::getOpenFileName(0, "Away File");
    if (!file.isEmpty())
        *awayFile = file;
}

void SetupPage::statCrewBrowse()
{
    QString file = QFileDialog::getOpenFileName(0, "StatCrew Game File");
    if (!file.isEmpty())
        *statCrew = file;
}

void SetupPage::awayColorDiag()
{
    QColor temp = QColorDialog::getColor(*awayColor, 0, "Away Color");
    if (temp.isValid()) {
        *awayColor = temp;
        awayColorPrev.fill(temp);
        awayColorBox->setPixmap(awayColorPrev);
    }
}

void SetupPage::homeColorDiag()
{
    QColor temp = QColorDialog::getColor(*homeColor, 0, "Home Color");
    if (temp.isValid()) {
        *homeColor = temp;
        homeColorPrev.fill(temp);
        homeColorBox->setPixmap(homeColorPrev);

    }
}

void SetupPage::bgDiag()
{
    QColor temp = QColorDialog::getColor(*bg, 0, "Background Color");
    if (temp.isValid())
        *bg = temp;
}

void SetupPage::logoBrowse() {
    QString file = QFileDialog::getOpenFileName(0, "Away Logo");
    if (!file.isEmpty())
        *awayLogo = file + "NOESPN";
}

void SetupPage::profileBrowse() {
    QString file = QFileDialog::getOpenFileName(0, "Away Profile",QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)+"/IMS Images/Logos_Keyable");
    if (!file.isEmpty()) {
        QFile csv(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)+"/IMS Images/Profiles.csv");
        csv.open(QIODevice::ReadOnly);
        QTextStream stream(&csv);
        while (!stream.atEnd()) {
            QStringList data = stream.readLine().split(',');
            QString name = file.mid(file.lastIndexOf('/')+1).split('.')[0];
            if (data[4] == name) {
                Profile p(data[1], data[2], data[3], data[0], file, QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)+"/IMS Images/Swatches/"+data[4]+".PNG");
                if (data[5] == "n") {
                    p.setLogoPath(p.getLogoPath() + "NOESPN");
                }
                activeProfile = p;
                applyProfile();
                csv.close();
                break;
            }
        }
    }
}

void SetupPage::showHelp()
{
    QMessageBox msg;
    msg.setText("Exit to Windows on the Tricaster. Open the start menu and type 'cmd' and press enter. "
                "When the command prompt opens, type 'ipconfig'."
                " Scroll to the top and find the 'IPv4 Address...' line under the wireless section.");
    msg.exec();
}

void SetupPage::applyProfile()
{
    if (!activeProfile.getLogoPath().isEmpty()) {
        awayNameLine.setText(activeProfile.getFullName());
        QImage swatch(activeProfile.getSwatchPath());
        switch (swatchSelector->currentIndex()) {
        case 0:
            *awayColor = swatch.pixel(0,10);
            break;
        default:
            *awayColor = swatch.pixel(0,14);
        }
        awayColorPrev.fill(*awayColor);
        awayColorBox->setPixmap(awayColorPrev);
        *awayLogo = activeProfile.getLogoPath();
        awayShortLine.setText(activeProfile.getShortName());
    }
}
