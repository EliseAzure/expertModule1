#include "term.h"

Term::Term()
{

}

Term::Term(QString name)
{
	this->name=name;
}

Term::Term(QString name, Plot plot)
{
	this->name=name;
	this->plot=plot;
}

Term::Term(QString name, Plot plot, double start, double end)
{
	this->name=name;
	this->plot=plot;
	this->start=start;
	this->end=end;
}

Term::~Term()
{

}

QString Term::getName()
{
	return name;
}

void Term::setName(QString name)
{
	this->name=name;
}

Plot Term::getPlot()
{
	return plot;
}

void Term::setPlot(Plot plot)
{
	this->plot=plot;
}

double Term::getStart()
{
	return start;
}

double Term::getEnd()
{
	return end;
}

void Term::setStart(double start)
{
	this->start=start;
}

void Term::setEnd(double end)
{
	this->end=end;
}
