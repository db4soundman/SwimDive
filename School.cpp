#include "School.h"

School::School(QString name, QColor primaryColor, QColor secondaryColor, QPixmap logo)
{
    this->name=name;
    this->primaryColor=primaryColor;
    this->secondaryColor=secondaryColor;
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
