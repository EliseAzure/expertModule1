#ifndef PLOTHANDLER
#define PLOTHANDLER

#include <ui_mainwindow.h>

class PlotHandler
{
public:
	PlotHandler();
	~PlotHandler();

	void updatePlot(Ui* ui, QVector<QPointF> points);
};

#endif // PLOTHANDLER

