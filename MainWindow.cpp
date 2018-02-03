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
                       SerialConsole *serial, ComparisonGraphic *comparisonGraphic, PastGamesGraphic* pgg, SwimMeet *meet, QWidget *parent)

    : QMainWindow(parent), panel(game, graphic, comGraphic, confSbGraphic, scheduleGraphic, comparisonGraphic), standingsPanel(graphic), nchcGui(confSbGraphic),
      awayPlayerEdit(game, false), homePlayerEdit(game, true), awayEdit(game->getAwayTeam()), homeEdit(game->getHomeTeam()),
      ltCreator(game->getLt()), compCreator(game), customLts(game->getLt(), game->getPreviewLt()),
      homePops(game,true), awayPops(game, false), homeLts(game, true),awayLts(game,false),
      sogUi(game), faceoffUi(game), customCompUi(game), clockControls(game, comGraphic, false),
      displayControls(game,graphic,comGraphic,confSbGraphic,scheduleGraphic,comparisonGraphic, pgg),
      goalies(game), ppCompUi(game), gameStateUi(game), awaypgUi(pgg, game->getAwayTeam(), false), homepgUi(pgg, game->getHomeTeam(), true),
      awayXmlHandler(game->getAwayTeam()), homeXmlHandler(game->getHomeTeam()),
      awayTextInput(game->getAwayTeam()), homeTextInput(game->getHomeTeam()), swimUi(meet)

{
    createAlternateContent();
    mainContent.addWidget(&alternateContent);
    mainContent.addWidget(&panel);
    setCentralWidget(&swimUi);
    connectWithCG(serial);

    makeMenu(game, serial, comGraphic);
    connect(&scheduleGui, SIGNAL(show(QList<ScheduleEntry>,bool)), scheduleGraphic, SLOT(receiveData(QList<ScheduleEntry>,bool)));
    connect(&scheduleGui, SIGNAL(show(QList<ScheduleEntry>,bool)), scheduleGraphic, SLOT(toggleShow()));
    connect(&treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(changeScreen(QModelIndex)));

    QStringList headers;
    headers << "Events";
    // Parse the meet file, grab event titles and separate with newlines.
    QFile file(":/resources/graphicsTree.txt");
    file.open(QIODevice::ReadOnly);
    model = new TreeModel(headers, file.readAll());
    file.close();
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
    if (modelMap.contains(model->data(index, Qt::DisplayRole).toString()))
        alternateContent.setCurrentIndex(modelMap[model->data(index, Qt::DisplayRole).toString()]);
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
    alternateContent.addWidget(&gameStateUi);
    alternateContent.addWidget(&awayPops);
    alternateContent.addWidget(&homePops);
    alternateContent.addWidget(&awayLts);
    alternateContent.addWidget(&homeLts);
    alternateContent.addWidget(&customLts);
    alternateContent.addWidget(&faceoffUi);
    alternateContent.addWidget(&sogUi);
    alternateContent.addWidget(&ppCompUi);
    alternateContent.addWidget(&customCompUi);
    alternateContent.addWidget(&nchcGui);
    alternateContent.addWidget(&standingsPanel);
    alternateContent.addWidget(&scheduleGui);
    alternateContent.addWidget(&awaypgUi);
    alternateContent.addWidget(&homepgUi);


    int x = 0;
    modelMap.insert("Game State", x++);
    modelMap.insert("Away Pops", x++);
    modelMap.insert("Home Pops", x++);
    modelMap.insert("Away LTs", x++);
    modelMap.insert("Home LTs", x++);
    modelMap.insert("Custom LTs", x++);
    modelMap.insert("Faceoff", x++);
    modelMap.insert("Shots On Goal", x++);
    modelMap.insert("Power play", x++);
    modelMap.insert("Custom", x++);
    modelMap.insert("NCHC Scoreboard", x++);
    modelMap.insert("NCHC Standings", x++);
    modelMap.insert("Upcoming Schedule", x++);
    modelMap.insert("Away Season History", x++);
    modelMap.insert("Home Season History", x++);

}
