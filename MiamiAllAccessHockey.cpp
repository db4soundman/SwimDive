#include "MiamiAllAccessHockey.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include "SetupWizard.h"
#include <QDesktopWidget>
#include <QAction>
#include <QTextStream>
#include "GraphicChooser.txt"
#include "SerialConsole.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QStringList>

MiamiAllAccessHockey::MiamiAllAccessHockey(int& argc, char* argv[]) :
    QApplication (argc, argv), mac("Mid-American Conference",QColor(0,167,92), QColor(0,42,92),
                                    QPixmap::fromImage(getTrimmedLogo(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)+"/IMS Images/Logos_Keyable/MAC.PNG"))) {
    setApplicationName("Swimming and Diving");
}

MiamiAllAccessHockey::~MiamiAllAccessHockey()
{
    if (usingTricaster) {
        delete tricaster;
    }
}

QString
MiamiAllAccessHockey::getAppDirPath() {
    return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + applicationName().replace(" ", "");
}
QString
MiamiAllAccessHockey::getPenaltiesFilePath() {
    return getAppDirPath() + "/penalties.txt";
}

QPixmap MiamiAllAccessHockey::getImgFromResources(QString name, int maxHeight, int maxWidth)
{
    QPixmap img(name);
    img = img.scaledToHeight(maxHeight, Qt::SmoothTransformation);

    if (img.width() > maxWidth) {
        return img.scaledToWidth(maxWidth, Qt::SmoothTransformation);
    }
    return img;
}

QPixmap MiamiAllAccessHockey::getImgFromESPN(QString name, int maxHeight, int maxWidth)
{
    QPixmap img = QPixmap::fromImage(getTrimmedLogo(name));
    img = img.scaledToHeight(maxHeight, Qt::SmoothTransformation);

    if (img.width() > maxWidth) {
        return img.scaledToWidth(maxWidth, Qt::SmoothTransformation);
    }
    return img;
}

void
MiamiAllAccessHockey::checkAppDirectory() {
    QDir appDir(getAppDirPath());
    if (!appDir.exists()) {
        appDir.mkdir(getAppDirPath());
    }

    QFile settings(getAppDirPath()+"/settings.txt");
    if (!settings.exists()) {
        QFile settingsSrc(":/resources/settings");
        settings.open(QIODevice::ReadWrite);
        QTextStream out(&settings);
        settingsSrc.open(QIODevice::ReadOnly);
        QTextStream in(&settingsSrc);
        out << in.readAll();
        settings.close();
    }

    params = Params((getAppDirPath() + "/settings.txt").toStdString());
}

int
MiamiAllAccessHockey::exec() {
    checkAppDirectory();

    QStringList schoolList;
//    schoolList << "AKRON" << "BALL_ST" << "BOWLING_GREEN" << "BUFFALO"
//               << "CENTRAL_MICHIGAN" << "EASTERN_MICHIGAN" << "EVANSVILLE" << "KENT_ST" << "MIAMI_OH"
//               << "MISSOURI_ST" << "NORTHERN_ILLINOIS" << "OHIO" << "SOUTHERN_ILLINOIS" << "TOLEDO" << "WESTERN_MICHIGAN";

    schoolList <<"BALL_ST" << "EASTERN_MICHIGAN" << "EVANSVILLE" << "MIAMI_OH"
               << "MISSOURI_ST" << "SOUTHERN_ILLINOIS";

    // Make vars, create wizard.
    scene = new QGraphicsScene();
    createSchools(schoolList);
    QString awayName, homeName, awayRank, homeRank, homeFile, awayFile, sponsor, announcer,
            goalies, statcrewName, awayLogo, tricasterIp, awayShort, homeShort;
    QColor awayColor, homeColor,  bg, hashtag;
    int pk, pkopp, ppg, ppopp, port;
    usingTricaster = true;
    homeColor.setRgb(226, 24, 54);
    bg.setRgb(0,120,0);
    announcer = params.stringValue("ANNOUNCER");
    sponsor = params.stringValue("SPONSOR");
    hashtag = params.stringValue("HASHTAG");
    homeName = "MIAMI";
    homeShort = "MIAMI";
    tricasterIp = params.stringValue("IP");
    QDesktopWidget desktop;

    SetupWizard wizard(&awayName, &homeName, &awayFile, &homeFile, &sponsor,
                       &announcer, &awayRank, &homeRank, &awayColor, &homeColor,
                       &bg, &pk, &pkopp, &ppg, &ppopp, &goalies, &statcrewName, &usingTricaster, &awayLogo,
                       &tricasterIp, &awayShort, &homeShort, &port);
    wizard.exec();
    QRect graphicsScreen = usingTricaster ? QRect(0,0,1920,1080) : desktop.screenGeometry(0);
    QPixmap awayLogoImg = QPixmap::fromImage(getTrimmedLogo(awayLogo));
    if (awayShort.trimmed().isEmpty()) {
        awayShort = awayName;
    }
    game = new HockeyGame(awayName, homeName, awayColor, homeColor,
                          awayFile, homeFile, sponsor, announcer, awayRank,
                          homeRank, graphicsScreen.width() + 1, awayLogoImg, homeShort, awayShort);
    if (usingTricaster)
        bg.setRgb(0,0,0);

    if (awayFile.isEmpty()) {
        HockeyTeam* team = game->getAwayTeam();
        team->setPk(pk);
        team->setPkopp(pkopp);
        team->setPpg(ppg);
        team->setPpopp(ppopp);
        team->setGoalies(goalies);
    }

    fullScreenGraphic.setX(graphicsScreen.width()/8);
    fullScreenGraphic.setY(100);

    scene->addItem(&fullScreenGraphic);

    pgg = new PastGamesGraphic(game->getHomeTeam(), game->getAwayTeam());
    pgg->setX(100);
    pgg->setY(650);
    scene->addItem(pgg);
    scene->addItem(game->getSb());

    scene->addItem(game->getLt());
    standings.setY(150);
    scene->addItem(&standings);
    scene->addItem(&nchcScoreboard);
    scene->addItem(&scheduleGraphic);
#ifdef GRADIENT_LOOK
    commercial = new CommercialGraphic(game, graphicsScreen.width(), awayLogo);
    game->getLt()->setX((graphicsScreen.width() / 2) - 214);
#else
    commercial = new CommercialGraphic(game, awayLogoImg);
    commercial->setX(graphicsScreen.width() / 2 - 500);
    game->getLt()->setX(0);
#endif
    scene->addItem(commercial);
    comparisonGraphic = new ComparisonGraphic(awayColor, homeColor, awayLogoImg);
    comparisonGraphic->setX((graphicsScreen.width() / 2) - 500);
    comparisonGraphic->setY(graphicsScreen.height() - 198);
    scene->addItem(comparisonGraphic);
    game->setComparisonGraphic(comparisonGraphic);
    scheduleGraphic.setX(100);
    scheduleGraphic.setY(650);
    game->getLt()->setY(graphicsScreen.height() - 160);
    game->getSb()->setY(graphicsScreen.height()-180);
    game->getSb()->setX((graphicsScreen.width()) - (game->getSb()->getRealWidth() + 100));
    commercial->setY(graphicsScreen.height() - 280);
    //commercial->setX(460);
    tv = new QGraphicsView(scene);



    tv->setGeometry(graphicsScreen);
    tv->setSceneRect(0, 0,graphicsScreen.width(), graphicsScreen.height());
    tv->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tv->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // Get desktop set up and wizard val later...
    tv->setBackgroundBrush(bg);
    //tv->setFixedSize(1600,900);
    //tv->show();
    tv->setFrameShape(QFrame::NoFrame);

    if (!statcrewName.isEmpty())
        stats = new StatCrewScanner(game, statcrewName);

    SerialConsole con;
    controlPanel = new MainWindow(game, &standings, commercial, &nchcScoreboard, &scheduleGraphic, &con, comparisonGraphic, pgg, &meet, &fullScreenGraphic);
    controlPanel->show();
    //game->connectWithSerialHandler(&con);

    if (!usingTricaster)
        tv->showFullScreen();
    else {
        previewWindow = new QWidget();
        QVBoxLayout* layout = new QVBoxLayout();
        tricaster = new TricasterHandler(tricasterIp, port, tv, bg);
        previewSb = new QGraphicsView(scene);
        previewSb->setBackgroundBrush(QColor(100,100,100));
        previewSb->setSceneRect(game->getSb()->x(), game->getSb()->y(), game->getSb()->getRealWidth(), 150);
        layout->addWidget(previewSb);
        previewLt = new QGraphicsView(scene);
        previewLt->setBackgroundBrush(QColor(100,100,100));
        previewLt->setSceneRect(300, 700, game->getSb()->getRealWidth(), 400);
        layout->addWidget(previewLt);
        previewWindow->setLayout(layout);
        previewWindow->setWindowTitle("Graphic View");
        previewWindow->show();
        game->getSb()->setUseTransparency(true);
        connect(scene, SIGNAL(changed(QList<QRectF>)), tricaster, SLOT(updatePortion(QList<QRectF>)));
        connect(game->getSb(), SIGNAL(transparentField(int,int,int,int)), tricaster, SLOT(addAlphaRect(int,int,int,int)));
        connect(game->getSb(), SIGNAL(removeTransparentField(int,int,int,int)), tricaster, SLOT(removeAlphaRect(int,int,int,int)));
        connect(game->getTricasterRefresh(), SIGNAL(timeout()), tricaster, SLOT(start()));
        connect(game->getSb(), SIGNAL(addNoTransparencyZone(QRect)), tricaster, SLOT(addNoTransparencyZone(QRect)));
        connect(commercial, SIGNAL(addNoTransparencyZone(QRect)), tricaster, SLOT(addNoTransparencyZone(QRect)));
        connect(game->getSb(),SIGNAL(removeNoTransparencyZone(QRect)), tricaster, SLOT(removeNoTransparencyZone(QRect)));
        connect(commercial,SIGNAL(removeNoTransparencyZone(QRect)), tricaster, SLOT(removeNoTransparencyZone(QRect)));

        connect(game->getLt(), SIGNAL(addNoTransparencyZone(QRect)), tricaster, SLOT(addNoTransparencyZone(QRect)));
        connect(game->getLt(),SIGNAL(removeNoTransparencyZone(QRect)), tricaster, SLOT(removeNoTransparencyZone(QRect)));

        connect(comparisonGraphic, SIGNAL(addNoTransparencyZone(QRect)), tricaster, SLOT(addNoTransparencyZone(QRect)));
        connect(comparisonGraphic,SIGNAL(removeNoTransparencyZone(QRect)), tricaster, SLOT(removeNoTransparencyZone(QRect)));
        connect(pgg, SIGNAL(addNoTransparencyZone(QRect)), tricaster, SLOT(addNoTransparencyZone(QRect)));
        connect(pgg,SIGNAL(removeNoTransparencyZone(QRect)), tricaster, SLOT(removeNoTransparencyZone(QRect)));
        connect(&fullScreenGraphic, SIGNAL(addNoTransparencyZone(QRect)), tricaster, SLOT(addNoTransparencyZone(QRect)));
        connect(&fullScreenGraphic,SIGNAL(removeNoTransparencyZone(QRect)), tricaster, SLOT(removeNoTransparencyZone(QRect)));
    }

    con.show();
    return QApplication::exec();
}

QImage MiamiAllAccessHockey::getTrimmedLogo(QString filePath)
{
    if (filePath.endsWith("NOESPN")) {
        filePath = filePath.left(filePath.indexOf("NOESPN"));
        QImage src(filePath);
        return src;
    }
    int tX, tY, bX, bY;
    tX = (tY = (bX = (bY = -1)));
    QImage src(filePath);
    QColor background = src.pixel(0,0);

    // Find top most pixel
    for (int i = 0; i < src.height() / 2 && tY == -1; i++) {
        for (int j = 0; j < src.width(); j++) {
            if (QColor(src.pixel(j, i)) != background) {
                tY = i;
                break;
            }
        }
    }

    // Find bottom most pixel
    for (int i = src.height() - 1; i > src.height() / 2 && bY == -1; i--) {
        for (int j = 0; j < src.width(); j++) {
            if (QColor(src.pixel(j, i)) != background) {
                bY = i;
                break;
            }
        }
    }

    // Find left most pixel
    for (int i = 0; i < src.width()/2 && tX == -1; i++) {
        for (int j = 0; j < src.height(); j++) {
            if (QColor(src.pixel(i, j)) != background) {
                tX = i;
                break;
            }
        }
    }

    // Find the right most pixel
    for (int i = src.width() - 1; i > src.width()/2 && bX == -1; i--) {
        for (int j = 0; j < src.height(); j++) {
            if (QColor(src.pixel(i, j)) != background) {
                bX = i;
                break;
            }
        }
    }
    QPoint tl(tX, tY);
    QPoint br(bX, bY);
    QRect box(tl, br);

    return src.copy(box);
}

School MiamiAllAccessHockey::getSchoolFromESPN(QString imsName)
{
     QFile csv(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)+"/IMS Images/Profiles.csv");
     Profile activeProfile;
     csv.open(QIODevice::ReadOnly);
     QTextStream stream(&csv);
     while (!stream.atEnd()) {
         QStringList data = stream.readLine().split(',');
         if (data[4] == imsName) {
             Profile p(data[1], data[2], data[3], data[0], QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)+"/IMS Images/Logos_Keyable/"+data[4]+".PNG",
                     QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)+"/IMS Images/Swatches/"+data[4]+".PNG");
             activeProfile = p;
             csv.close();
             break;
         }
     }
     if (!activeProfile.getLogoPath().isEmpty()) {
         QImage swatch(activeProfile.getSwatchPath());
         return School(activeProfile.getFullName(),swatch.pixel(0,10),swatch.pixel(0,14),QPixmap(activeProfile.getLogoPath()));
     }
     return School();
}

School MiamiAllAccessHockey::getSwatchFromESPN(QString imsName)
{
    QImage swatch(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)+"/IMS Images/Swatches/"+imsName+".PNG");
    return School("Mid-American Conference",swatch.pixel(0,10),swatch.pixel(0,14),
                  QPixmap::fromImage(getTrimmedLogo(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)+"/IMS Images/Logos_Keyable/"+imsName+".PNG")));
}

void
MiamiAllAccessHockey::createSchools(QStringList schoolNames)
{
    QList<School*> schools;
    QFile csv(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)+"/IMS Images/Profiles.csv");
    Profile activeProfile;
    for (int i=0; i < schoolNames.length(); i++) {
        QString name = schoolNames[i];
        csv.open(QIODevice::ReadOnly);
        QTextStream stream(&csv);
        while (!stream.atEnd()) {
            QStringList data = stream.readLine().split(',');
            if (data[4] == name) {
                Profile p(data[1], data[2], data[3], data[0], QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)+"/IMS Images/Logos_Keyable/"+data[4]+".PNG",
                        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)+"/IMS Images/Swatches/"+data[4]+".PNG");
                activeProfile = p;
                csv.close();
                break;
            }
        }
        if (!activeProfile.getLogoPath().isEmpty()) {
            QImage swatch(activeProfile.getSwatchPath());
            schools.append(new School(activeProfile.getFullName(),swatch,QPixmap::fromImage(getTrimmedLogo(activeProfile.getLogoPath()))));
        }
    }
    meet.setSchools(schools);
}

