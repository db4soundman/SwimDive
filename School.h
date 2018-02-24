#ifndef SCHOOL_H
#define SCHOOL_H

#include <QObject>
#include <QPixmap>
#include <QColor>
#include <QImage>

class School
{
public:
    School();
    School(QString name, QColor primaryColor, QColor secondaryColor, QPixmap logo);
    School(QString name, QImage swatch, QPixmap logo);

    QString getName() const;
    void setName(const QString &value);

    QColor getPrimaryColor() const;
    void setPrimaryColor(const QColor &value);

    QColor getSecondaryColor() const;
    void setSecondaryColor(const QColor &value);

    QPixmap getLogo() const;
    void setLogo(const QPixmap &value);

    QColor getPrimaryLogoBg() const;
    void setPrimaryLogoBg(const QColor &value);

    QColor getSecondaryLogoBg() const;
    void setSecondaryLogoBg(const QColor &value);

private:
    QString name;
    QColor primaryColor, secondaryColor, primaryLogoBg, secondaryLogoBg;
    QPixmap logo;
};

#endif // SCHOOL_H
