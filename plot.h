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
		void addPointBack(QPointF point);
		void clearPoints();
		void erasePoint(int n);

	private:
		QVector<QPointF> points;
		QString name;
};

#endif // PLOT

