#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <stdlib.h>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	//Plot stuff
	ui->plot->setInteractions(QCP::iRangeZoom | QCP::iSelectPlottables);
	ui->plot->xAxis->setRange(-10, 10);
	ui->plot->yAxis->setRange(-10, 10);

	//ui->plot->plotLayout()->insertRow(0);
	//ui->plot->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->plot, "Interaction Example"));
	//ui->plot->xAxis->setLabel("x Axis");
	//ui->plot->yAxis->setLabel("y Axis");
	//~Plot stuff


	//PointList stuff
	//Adjust column sizes(shitcoded)
	//!TODO: replace with accurate methods
	ui->points_list->setColumnWidth(0,ui->points_list->width()*0.4);
	ui->points_list->setColumnWidth(1,ui->points_list->width()*0.4);
	ui->points_list->horizontalHeader()->setStretchLastSection(true);
	//Edit and remove headers
	ui->points_list->setHorizontalHeaderItem(0,new QTableWidgetItem("x"));
	ui->points_list->setHorizontalHeaderItem(1,new QTableWidgetItem("y"));
	ui->points_list->setHorizontalHeaderItem(2,new QTableWidgetItem(""));
	ui->points_list->verticalHeader()->setVisible(false);
	//~PointList stuff

	connect(ui->plot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(onPlotClicked()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::onPlotClicked()
{
	double dotSize=0.05;
	QPoint p = ui->plot->mapFromGlobal(QCursor::pos());

	//Get click coordinates in plot scale
	double x = ui->plot->xAxis->pixelToCoord(p.x());
	double y = ui->plot->yAxis->pixelToCoord(p.y());
	//Log coordinates
	std::cout<<x<<"\t"<<y<<std::endl;
	//Place a point on the plot
	QCPItemEllipse *dot = new QCPItemEllipse(ui->plot);
	ui->plot->addItem(dot);
	dot->topLeft->setCoords(x-dotSize,y+dotSize);
	dot->bottomRight->setCoords(x+dotSize,y-dotSize);
	ui->plot->replot();
}

void MainWindow::on_button_new_clicked()
{
	//Clear the plot
	ui->plot->clearItems();
	ui->plot->replot();

	//Clear the list
	ui->points_list->setRowCount(0);

	//Add items to list
	//!TODO: See if I need to free this memory(it doesn't work if I do)
	ui->points_list->insertRow(0);
	QTableWidgetItem *item=new QTableWidgetItem;
	//Add plus buttom
	QPushButton *button=new QPushButton;
	button->setText("+");
	connect(button,SIGNAL(clicked()),this,SLOT(addPointItem()));
	ui->points_list->setItem(0,0,item);
	ui->points_list->setCellWidget(0,0,button);
	ui->points_list->setSpan(0,0,1,3);
	//Add two first dots
	for(int i=0; i<2; i++)
	{
		addPointItem();
	}
}

void MainWindow::removePointItem()
{
	//Get sender row
	int row=sender()->property("row").toInt();
	//Remove this row
	ui->points_list->removeRow(row);
	//Decrement all above button properties for further usages
	for(int i=row;i<ui->points_list->rowCount()-1;i++)
	{
		ui->points_list->cellWidget(i,2)->setProperty("row",ui->points_list->cellWidget(i,2)->property("row").toInt()-1);
	}
}

void MainWindow::addPointItem(double x, double y)
{
	//Insert new row before plus button
	ui->points_list->insertRow(ui->points_list->rowCount()-1);
	//Add box for x
	QTableWidgetItem *item=new QTableWidgetItem;
	QLineEdit *line=new QLineEdit;
	line->setText(QString::number(x));
	ui->points_list->setItem(ui->points_list->rowCount()-2, 0, item);
	ui->points_list->setCellWidget(ui->points_list->rowCount()-2, 0, line);
	//Add box for y
	item=new QTableWidgetItem;
	line=new QLineEdit;
	line->setText(QString::number(y));
	ui->points_list->setItem(ui->points_list->rowCount()-2, 1, item);
	ui->points_list->setCellWidget(ui->points_list->rowCount()-2, 1, line);
	//Add minus button
	item=new QTableWidgetItem;
	QPushButton *button=new QPushButton;
	button->setProperty("row",ui->points_list->rowCount()-2);
	button->setText("-");
	connect(button,SIGNAL(clicked()),this,SLOT(removePointItem()));
	ui->points_list->setItem(ui->points_list->rowCount()-2, 2, item);
	ui->points_list->setCellWidget(ui->points_list->rowCount()-2, 2, button);
	ui->points_list->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::addPointItem()
{
	addPointItem(0, 0);
}

void MainWindow::on_button_clear_item_clicked()
{
	//Clear the plot
	ui->plot->clearItems();
	ui->plot->replot();
	//Clear the list
	ui->points_list->clear();
	ui->points_list->setRowCount(0);
}
