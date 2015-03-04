#include "saveload.h"
#include <QFile>
#include <QTextStream>
#include <QDomDocument>
#include <iostream>
#include <QDebug>

SaveLoad::SaveLoad()
{

}

SaveLoad::~SaveLoad()
{

}

void SaveLoad::save(QVector<Plot> models, QString filename)
{
	QDomDocument doc("MODELS");

	QDomElement modelElements = doc.createElement("models");

	for( int i = 0; i<models.length(); i++ )
	{
		QDomElement modelElement = doc.createElement("model");
		modelElement.setAttribute("id", i);
		modelElements.appendChild(modelElement);

		QDomElement modelName = doc.createElement("name");
		modelName.appendChild(doc.createTextNode(models[i].getName()));
		modelElement.appendChild(modelName);

		QDomElement modelPoints = doc.createElement("points");
		for(int j=0;j<models[i].getPoints().length();j++)
		{
			QDomElement point = doc.createElement("point");
			point.setAttribute("id", j);
			QDomElement x = doc.createElement("x");
			QDomText xText = doc.createTextNode(QString::number(models[i].getPoints()[j].x()));
			x.appendChild(xText);
			QDomElement y = doc.createElement("y");
			QDomText yText = doc.createTextNode(QString::number(models[i].getPoints()[j].y()));
			y.appendChild(yText);
			point.appendChild(x);
			point.appendChild(y);
			modelPoints.appendChild(point);
		}
		modelElement.appendChild(modelPoints);
	}
	doc.appendChild(modelElements);

	QFile file(filename);
	if(file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream(&file) << doc.toString(3);
		file.close();
	}
}

void traverseNode(const QDomNode& node, QVector<Plot>* models)
{
	QDomNode domNode = node.firstChild();
	while(!domNode.isNull() && node.toElement().tagName()=="models")
	{
		if(domNode.isElement())
		{
			QDomElement domElement = domNode.toElement();
			if(!domElement.isNull() && domElement.tagName() == "model")
			{
				//qDebug() << "model attr: " << domElement.attribute("id", "");
				Plot model;


				QDomNode domN = domElement.firstChild();
				while(!domN.isNull())
				{
					if(domN.isElement())
					{
						QDomElement domEl = domN.toElement();
						if(domEl.tagName()=="name")
						{
							//qDebug() << "model name: " << domEl.text();
							model.setName(domEl.text());
							models->push_back(model);
						}
						else if(domEl.tagName()=="points")
						{
							//qDebug() << "points: ";
							QVector<QPointF> points;
							double x, y;

							QDomNode point = domEl.firstChild();

							while(!point.isNull())
							{
								if(point.isElement())
								{
									//qDebug() << "point attr: "<<point.toElement().attribute("id", "");
									if(point.childNodes().length()==2)
									{
										x=point.toElement().firstChild().toElement().text().toDouble();
										y=point.toElement().lastChild().toElement().text().toDouble();
										//qDebug() << "x: " << QString::number(x);
										//qDebug() << "y: " << QString::number(y);
										points.push_back(QPointF(x,y));
									}
								}
								point=point.nextSibling();
							}
							models->last().setPoints(points);
						}
					}
					domN=domN.nextSibling();
				}
			}
		}
		domNode = domNode.nextSibling();
	}
}

void SaveLoad::load(QVector<Plot>* models, QString filename)
{
	QDomDocument doc;
	QFile file(filename);
	qDebug()<<"1";
	if(file.open(QIODevice::ReadOnly))
	{
		qDebug()<<"2";
		QString errorStr;
		int errorLine;
		int errorColumn;
		if(doc.setContent(&file, true, &errorStr, &errorLine,	&errorColumn))
		{
			qDebug()<<"3";
			models->clear();
			QDomElement domElement = doc.documentElement();
			traverseNode(domElement, models);
		}
		else
			qDebug()<<errorStr;
		file.close();
	}
}
