#ifndef MIAMIALLACCESSHOCKEY_H
#define MIAMIALLACCESSHOCKEY_H
//#define DEBUG
#include <QApplication>
#include "HockeyGame.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include "MainWindow.h"
#include "StandingsGraphic.h"
#include "CommercialGraphic.h"
#include "NchcScoreboardGraphic.h"
#include "StatCrewScanner.h"
#include "TricasterHandler.h"
#include "schedulegraphic.h"
#include "Params.h"
#include "ComparisonGraphic.h"
#include <QImage>
#include <QPixmap>
#include <QWidget>
#include "CGSimulator.h"
#include "PastGamesGraphic.h"
#include "SwimMeet.h"
#include "FullScreenGraphic.h"

class MiamiAllAccessHockey : public QApplication {
public:
    MiamiAllAccessHockey(int &argc, char *argv[]);
    ~MiamiAllAccessHockey();
    /**
     * @brief getAppDirPath Gets the file path to the application
     * directory.
     * @return A QString representing the file path to the application
     * directory.
     */
    static QString getAppDirPath();

    static QString getPenaltiesFilePath();

    static QPixmap getImgFromResources(QString name, int maxHeight, int maxWidth = 1920);
    static QPixmap getImgFromESPN(QString name, int maxHeight, int maxWidth = 1920);

    void checkAppDirectory();

    int exec();

    static QImage getTrimmedLogo(QString filePath);

    static School getSchoolFromESPN(QString imsName);
    static School getSwatchFromESPN(QString imsName);


private:
    QGraphicsView* tv, *previewSb, *previewLt;
    QGraphicsScene* scene;
    HockeyGame* game;
    MainWindow* controlPanel;
    StandingsGraphic standings;
    NchcScoreboardGraphic nchcScoreboard;
    ScheduleGraphic scheduleGraphic;
    CommercialGraphic* commercial;
    ComparisonGraphic* comparisonGraphic;
    StatCrewScanner* stats;
    TricasterHandler* tricaster;
    PastGamesGraphic* pgg;
    Params params;
    QWidget* previewWindow;
    bool usingTricaster;

    SwimMeet meet;
    School mac;

    FullScreenGraphic fullScreenGraphic;

    void createSchools(QStringList schoolNames);

#ifdef DEBUG
    CGSimulator cgSimulator;
#endif
};

#endif // MIAMIALLACCESSHOCKEY_H
