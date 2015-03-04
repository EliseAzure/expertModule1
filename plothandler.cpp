#include "plothandler.h"

PlotHandler::PlotHandler()
{

}

PlotHandler::~PlotHandler()
{

}

void PlotHandler::updatePlot(Ui* ui,QVector<QPointF> points)
{
	ui->button_clear_model->setText("lalalal");
//	//! TODO: avoid extra plot (may be it's unnecessary)
//	//Clear and update the plot
//	ui->plot->clearItems();
//	ui->plot->replot();
//	// place first point on the plot. There is not enough points to plot lines
//	double dotSize=0.08;
//	QCPItemEllipse *dot = new QCPItemEllipse(ui->plot);
//	ui->plot->addItem(dot);
//	dot->topLeft->setCoords(points[0].x()-dotSize, points[0].y()+dotSize);
//	dot->bottomRight->setCoords(points[0].x()+dotSize, points[0].y()-dotSize);
//	//Plot lines and other points
//	if(points.size() >= 2)
//	{
//		for (auto i = points.begin() + 1 ; i != points.end(); ++i)
//		{
//			//Point stuff
//			dot = new QCPItemEllipse(ui->plot);
//			ui->plot->addItem(dot);
//			dot->topLeft->setCoords((*i).x()-dotSize, (*i).y()+dotSize);
//			dot->bottomRight->setCoords((*i).x()+dotSize, (*i).y()-dotSize);
//			//Line stuff
//			QCPItemLine *line = new QCPItemLine(ui->plot);
//			ui->plot->addItem(line);
//			line->start->setCoords(*(i - 1));
//			line->end->setCoords(*i);
//			line->setPen(QPen(QColor(0, 148, 255)));
//		}
//	}
//	ui->plot->replot();
}
