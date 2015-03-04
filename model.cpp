#include "model.h"

Model::Model()
{

}

Model::Model(QString name)
{
	this->name=name;
}

Model::Model(QString name, QVector<Term> terms)
{
	this->name=name;
	this->terms=terms;
}

Model::Model(QString name, QVector<Term> terms, double rangeStart, double rangeEnd)
{
	this->name=name;
	this->terms=terms;
	this->rangeStart=rangeStart;
	this->rangeEnd=rangeEnd;
}

Model::~Model()
{

}

void Model::setName(QString name)
{
	this->name=name;
}

QString Model::getName()
{
	return name;
}

void Model::setTerms(QVector<Term> terms)
{
	this->terms=terms;
}

QVector<Term> Model::getTerms()
{
	return terms;
}

void Model::setRangeStart(double rangeStart)
{
	this->rangeStart=rangeStart;
}

void Model::setRangeEnd(double rangeEnd)
{
	this->rangeEnd=rangeEnd;
}

void Model::setRange(double rangeStart, double rangeEnd)
{
	this->rangeStart=rangeStart;
	this->rangeEnd=rangeEnd;
}

double Model::getRangeStart()
{
	return rangeStart;
}

double Model::getRangeEnd()
{
	return rangeEnd;
}

void Model::clearTerms()
{
	terms.clear();
}
