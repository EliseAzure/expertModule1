#include "model.h"

Model::Model()
{

}

Model::Model(QString name)
{
	this->name=name;
}

Model::Model(QString name, QVector<QPointF> points)
{
	this->name=name;
	this->points=points;
}

Model::~Model()
{
	name.clear();
	points.clear();
}

QVector<QPointF> Model::getPoints()
{
	return points;
}

void Model::setPoints(QVector<QPointF> points)
{
	this->points=points;
}

QString Model::getName()
{
	return name;
}

void Model::setName(QString name)
{
	this->name=name;
}
