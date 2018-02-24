#include "School.h"

School::School()
{

}

School::School(QString name, QColor primaryColor, QColor secondaryColor, QPixmap logo)
{
    this->name=name;
    this->primaryColor=primaryColor;
    this->secondaryColor=secondaryColor;
    this->logo=logo;
}

School::School(QString name, QImage swatch, QPixmap logo)
{
    this->name=name;
    this->primaryLogoBg = swatch.pixel(0,2);
    this->secondaryLogoBg= swatch.pixel(0,14);
    this->primaryColor=swatch.pixel(0,5);
    this->secondaryColor=swatch.pixel(6,5);
    this->logo=logo;

}

QString School::getName() const
{
    return name;
}

void School::setName(const QString &value)
{
    name = value;
}

QColor School::getPrimaryColor() const
{
    return primaryColor;
}

void School::setPrimaryColor(const QColor &value)
{
    primaryColor = value;
}

QColor School::getSecondaryColor() const
{
    return secondaryColor;
}

void School::setSecondaryColor(const QColor &value)
{
    secondaryColor = value;
}

QPixmap School::getLogo() const
{
    return logo;
}

void School::setLogo(const QPixmap &value)
{
    logo = value;
}

QColor School::getPrimaryLogoBg() const
{
    return primaryLogoBg;
}

void School::setPrimaryLogoBg(const QColor &value)
{
    primaryLogoBg = value;
}

QColor School::getSecondaryLogoBg() const
{
    return secondaryLogoBg;
}

void School::setSecondaryLogoBg(const QColor &value)
{
    secondaryLogoBg = value;
}
