#ifndef MODEL
#define MODEL

#include "term.h"

class Model
{
	public:
		Model();
		Model(QString name);
		Model(QString name, QVector<Term> terms);
		Model(QString name, QVector<Term> terms, double rangeStart, double rangeEnd);
		~Model();
		void setName(QString name);
		QString getName();
		void setTerms(QVector<Term> terms);
		QVector<Term> getTerms();
		void setRangeStart(double rangeStart);
		void setRangeEnd(double rangeEnd);
		void setRange(double rangeStart, double rangeEnd);
		double getRangeStart();
		double getRangeEnd();
		void clearTerms();

	private:
		QString name;
		QVector<Term> terms;
		double rangeStart, rangeEnd;

};

#endif // MODEL

