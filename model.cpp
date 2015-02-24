#include "model.h"

#include <QStringList>

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

QString Model::toString()
{
	QString res="";
	res.append("<name:=").append(name).append(">\n");
	for(auto i=points.begin();i<points.end();i++)
	{
		res.append("<x:=").append(QString::number((*i).x())).append(">");
		res.append("<y:=").append(QString::number((*i).y())).append(">\n");
	}
	return res;
}

void Model::fromString(QString asString)
{
	QStringList list, xy;
	double x, y;
	list=asString.split("\n");
	name=list.value(0).mid(7,list.value(0).length()-8);
	for(int i=1;i<list.length()-1;i++)
	{
		xy=list.value(i).split(">");
		x=xy.value(0).mid(4,xy.value(0).length()-4).toDouble();
		y=xy.value(1).mid(4,xy.value(1).length()-4).toDouble();
		points.push_back(QPointF(x, y));
	}
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
