#ifndef TERM
#define TERM

#include "plot.h"

class Term
{
	public:
		Term();
		Term(QString name);
		Term(QString name, Plot plot);
		Term(QString name, Plot plot, double start, double end);
		~Term();
		void setName(QString name);
		QString getName();
		void setPlot(Plot plot);
		Plot getPlot();
		void setStart(double start);
		void setEnd(double end);
		double getStart();
		double getEnd();

	private:
		Plot plot;
		QString name;
		double start, end;
};

#endif // TERM

