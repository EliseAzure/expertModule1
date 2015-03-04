#ifndef PLOT
#define PLOT

#include <QVector>
#include <QString>
#include <QPointF>

class Plot
{
	public:
		Plot();
		Plot(QString name);
		Plot(QString name, QVector<QPointF> points);
		~Plot();
		void setPoints(QVector<QPointF> points);
		QVector<QPointF> getPoints();
		void setName(QString name);
		QString getName();

	private:
		QVector<QPointF> points;
		QString name;
};

#endif // PLOT

