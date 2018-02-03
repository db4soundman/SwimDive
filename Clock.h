#ifndef CLOCK_H
#define CLOCK_H

#include <QObject>
#include <QString>
#include <QTime>

class Clock : public QObject {
    Q_OBJECT
public:
    Clock();
    Clock(int minutes, bool serialPowerPlay=false);

    void setClock(int m, int s, int t);
    void setClock(QString serialString);
    void setClockDebug(QString serialString);

    QString toString();
    QString toStringPP();
    QString toStringPPDebug();
    QString toStringDebug();
    QString getTimeSincePdStarted();
    QString getTimeSinceOtStarted();
    int getTimeLeft();

    bool isGameClock() const;
    void setGameClock(bool value);

    int getMinutes() const;
    void setMinutes(int value);

    int getSeconds() const;
    void setSeconds(int value);

    int getTenths() const;
    void setTenths(int value);

public slots:
    void tick();
    void resetClock(bool ot = false);
    void usingSerialClock();
    void noLongerUsingSerialClock();

signals:
    void clockExpired();
    void clockUpdated();

private:
    int minutes, seconds, tenths;
    int regulationLength;
    int otLength;
    bool gameClock, useSerial, serialPP;
    QTime serial;
    QString cgClockString;
};

#endif // CLOCK_H
