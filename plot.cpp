#include "plot.h"

Plot::Plot()
{

}

Plot::Plot(QString name)
{
	this->name=name;
}

Plot::Plot(QString name, QVector<QPointF> points)
{
	this->name=name;
	this->points=points;
}

Plot::~Plot()
{

}

QVector<QPointF> Plot::getPoints()
{
	return points;
}

void Plot::setPoints(QVector<QPointF> points)
{
	this->points=points;
}

QString Plot::getName()
{
	return name;
}

void Plot::setName(QString name)
{
	this->name=name;
}

