#include "MainWindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QFileDialog>

MainWindow::MainWindow(HockeyGame* game, StandingsGraphic* graphic, CommercialGraphic* comGraphic,
                       NchcScoreboardGraphic* confSbGraphic, ScheduleGraphic *scheduleGraphic,
                       SerialConsole *serial, ComparisonGraphic *comparisonGraphic, PastGamesGraphic* pgg, SwimMeet *meet, FullScreenGraphic* fullScreenGraphic, QWidget *parent)

    : QMainWindow(parent), panel(game, graphic, comGraphic, confSbGraphic, scheduleGraphic, comparisonGraphic), standingsPanel(graphic), nchcGui(confSbGraphic),
      awayPlayerEdit(game, false), homePlayerEdit(game, true), awayEdit(game->getAwayTeam()), homeEdit(game->getHomeTeam()),
      ltCreator(game->getLt()), compCreator(game), customLts(game->getLt(), game->getPreviewLt()),
      homePops(game,true), awayPops(game, false), homeLts(game, true),awayLts(game,false),
      sogUi(game), faceoffUi(game), customCompUi(game), clockControls(game, comGraphic, false),
      displayControls(game,graphic,comGraphic,confSbGraphic,scheduleGraphic,comparisonGraphic,fullScreenGraphic, pgg),
      goalies(game), ppCompUi(game), gameStateUi(game), awaypgUi(pgg, game->getAwayTeam(), false), homepgUi(pgg, game->getHomeTeam(), true),
      awayXmlHandler(game->getAwayTeam()), homeXmlHandler(game->getHomeTeam()),
      awayTextInput(game->getAwayTeam()), homeTextInput(game->getHomeTeam()), swimUi(meet, 1, game,true)

{
    QString titles = "";
    for (int i = 0; i < 10; i++) {
        eventUis.append(new SwimEventUI(meet,i+1,game));
        connect(eventUis.last(), SIGNAL(showLanes(QList<Swimmer>,QString)), fullScreenGraphic, SLOT(showLaneAssignments(QList<Swimmer>,QString)));
        connect(eventUis.last(), SIGNAL(showResults(QList<Swimmer>,QString)), fullScreenGraphic, SLOT(showResults(QList<Swimmer>,QString)));
        connect(eventUis.last(), SIGNAL(showTimes(QList<Swimmer>,QString)), fullScreenGraphic, SLOT(showResultsWithTime(QList<Swimmer>,QString)));

        connect(eventUis.last(), SIGNAL(showDiveLanes(QList<Diver>,QString)), fullScreenGraphic, SLOT(showDiveOrder(QList<Diver>,QString)));
        connect(eventUis.last(), SIGNAL(showDiveResults(QList<Diver>,QString)), fullScreenGraphic, SLOT(showDivingResults(QList<Diver>,QString)));
        titles += "Swimming Event " + QString::number(i+1) + "\n";
    }
    for (int i = 0; i < 10; i++) {
        eventUis.append(new SwimEventUI(meet,i+1,game, true));
        connect(eventUis.last(), SIGNAL(showLanes(QList<Swimmer>,QString)), fullScreenGraphic, SLOT(showLaneAssignments(QList<Swimmer>,QString)));
        connect(eventUis.last(), SIGNAL(showResults(QList<Swimmer>,QString)), fullScreenGraphic, SLOT(showResults(QList<Swimmer>,QString)));
        connect(eventUis.last(), SIGNAL(showTimes(QList<Swimmer>,QString)), fullScreenGraphic, SLOT(showResultsWithTime(QList<Swimmer>,QString)));

        connect(eventUis.last(), SIGNAL(showDiveLanes(QList<Diver>,QString)), fullScreenGraphic, SLOT(showDiveOrder(QList<Diver>,QString)));
        connect(eventUis.last(), SIGNAL(showDiveResults(QList<Diver>,QString)), fullScreenGraphic, SLOT(showDivingResults(QList<Diver>,QString)));
        titles += "Diving Event " + QString::number(i+1) + "\n";
    }
    createAlternateContent();
    mainContent.addWidget(&alternateContent);
    mainContent.addWidget(&panel);
    setCentralWidget(&mainContent);
    connectWithCG(serial);



    makeMenu(game, serial, comGraphic);
    connect(&scheduleGui, SIGNAL(show(QList<ScheduleEntry>,bool)), scheduleGraphic, SLOT(receiveData(QList<ScheduleEntry>,bool)));
    connect(&scheduleGui, SIGNAL(show(QList<ScheduleEntry>,bool)), scheduleGraphic, SLOT(toggleShow()));
    connect(&treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(changeScreen(QModelIndex)));

    QStringList headers;
    headers << "Events";
    // Parse the meet file, grab event titles and separate with newlines.
//    QFile file(":/resources/graphicsTree.txt");
//    file.open(QIODevice::ReadOnly);
    model = new TreeModel(headers, titles);
   // file.close();
    treeView.setModel(model);
    treeView.setAlternatingRowColors(true);
    treeView.setHeaderHidden(true);
    treeView.setAnimated(true);
    leftDock.setWindowTitle("Graphic Selector");
    leftDock.setFeatures(0);
    leftDock.setWidget(&treeView);
    addDockWidget(Qt::LeftDockWidgetArea, &leftDock);

    rightDock.setFeatures(0);
    rightDock.setWidget(&clockControls);
    addDockWidget(Qt::RightDockWidgetArea, &rightDock);

    topDock.setFeatures(0);
    topDock.setWidget(&displayControls);
    addDockWidget(Qt::TopDockWidgetArea, &topDock);


    //treeView.setEnabled(false);

}

MainWindow::~MainWindow() {

}

void MainWindow::connectWithCG(SerialConsole *con)
{
    connect(con, SIGNAL(dataReceived(QByteArray)), &gameStateUi, SLOT(updateData(QByteArray)));
}

void MainWindow::connectWithSerialSimulator(CGSimulator *console)
{
    connect(console, SIGNAL(dataReceived(QByteArray)), &gameStateUi, SLOT(updateData(QByteArray)));
}

void MainWindow::changeScreen(QModelIndex index)
{
    if (modelMap.contains(model->data(index, Qt::DisplayRole).toString())) {
        alternateContent.setCurrentIndex(modelMap[model->data(index, Qt::DisplayRole).toString()]);
        eventUis[modelMap[model->data(index, Qt::DisplayRole).toString()]]->sendEventToScoreboard();
    }
}

void MainWindow::switchContent()
{
    if (mainContent.currentIndex() == 0) {
        mainContent.setCurrentIndex(1);
    } else {
        mainContent.setCurrentIndex(0);
    }

    treeView.setEnabled(mainContent.currentIndex() == 0);
}

void MainWindow::updateAwayRoster()
{
    QString file = QFileDialog::getOpenFileName(0, "Away File");
    if (!file.isEmpty())
        awayXmlHandler.parseFile(file, true);
}

void MainWindow::updateHomeRoster()
{
    QString file = QFileDialog::getOpenFileName(0, "Home File");
    if (!file.isEmpty())
        homeXmlHandler.parseFile(file, true);
}

void MainWindow::makeMenu(HockeyGame* game, SerialConsole* console, CommercialGraphic* comGraphic)
{

}

void MainWindow::createAlternateContent()
{
    for (int i = 0; i < eventUis.length(); i++) {
        alternateContent.addWidget(eventUis[i]);
    }


    int x = 0;

    for (int i = 0; i < 10; i++) {
        modelMap.insert("Swimming Event " + QString::number(i+1), x++);
    }
    for (int i = 0; i < 10; i++) {
       modelMap.insert("Diving Event " + QString::number(i+1), x++);
    }
//    modelMap.insert("Game State", x++);
//    modelMap.insert("Away Pops", x++);
//    modelMap.insert("Home Pops", x++);
//    modelMap.insert("Away LTs", x++);
//    modelMap.insert("Home LTs", x++);
//    modelMap.insert("Custom LTs", x++);
//    modelMap.insert("Faceoff", x++);
//    modelMap.insert("Shots On Goal", x++);
//    modelMap.insert("Power play", x++);
//    modelMap.insert("Custom", x++);
//    modelMap.insert("NCHC Scoreboard", x++);
//    modelMap.insert("NCHC Standings", x++);
//    modelMap.insert("Upcoming Schedule", x++);
//    modelMap.insert("Away Season History", x++);
//    modelMap.insert("Home Season History", x++);

}
