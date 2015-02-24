#ifndef MODEL
#define MODEL

#include <QVector>
#include <QString>
#include <QPointF>

class Model
{
	public:
		Model();
		Model(QString name);
		Model(QString name, QVector<QPointF> points);
		~Model();
		void setPoints(QVector<QPointF> points);
		QVector<QPointF> getPoints();
		void setName(QString name);
		QString getName();
		QString toString();
		void fromString(QString asString);

	private:
		QVector<QPointF> points;
		QString name;
};

#endif // MODEL

