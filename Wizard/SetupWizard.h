#ifndef SETUPWIZARD_H
#define SETUPWIZARD_H

#include <QWizard>
#include <QPushButton>
#include <QColor>
#include "SetupPage.h"
#include "AwayTeamInfo.h"

class SetupWizard : public QWizard {
public:
    SetupWizard(QString* pAwayName, QString* pHomeName, QString* pAwayFile, QString* pHomeFile,
                QString* pSponsor, QString* pAnnouncer, QString* pAwayRank, QString* pHomeRank,
                QColor* pAwayColor, QColor* pHomeColor, QColor* pBg, int* pk, int* pkopp,
                int* ppg, int* ppopp, QString* goalies, QString* statcrewName, bool* usingTricaster,
                QString *awayLogo, QString *tricasterIp, QString* aSname, QString* hSname, int *port);

private:
    SetupPage wizpage;
    AwayTeamInfo ati;
};

#endif // SETUPWIZARD_H
