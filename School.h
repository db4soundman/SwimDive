#ifndef SCHOOL_H
#define SCHOOL_H

#include <QObject>
#include <QPixmap>
#include <QColor>

class School
{
public:
    School();
    School(QString name, QColor primaryColor, QColor secondaryColor, QPixmap logo);

    QString getName() const;
    void setName(const QString &value);

    QColor getPrimaryColor() const;
    void setPrimaryColor(const QColor &value);

    QColor getSecondaryColor() const;
    void setSecondaryColor(const QColor &value);

    QPixmap getLogo() const;
    void setLogo(const QPixmap &value);

private:
    QString name;
    QColor primaryColor, secondaryColor;
    QPixmap logo;
};

#endif // SCHOOL_H
