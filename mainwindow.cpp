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

    ui->plot->setDisabled(true);

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

    // For add points
    connect(ui->plot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(onPlotClicked()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::onPlotClicked()
{
    QPoint p = ui->plot->mapFromGlobal(QCursor::pos());

    //Get click coordinates in plot scale
    double x = ui->plot->xAxis->pixelToCoord(p.x());
    double y = ui->plot->yAxis->pixelToCoord(p.y());
    coordOfDots.push_back(QPointF(x, y));

    //Log coordinates
    std::cout << x << "\t" << y << std::endl;

    toPlot();
    // add to list
    QWidget* item = ui->points_list->cellWidget(coordOfDots.size(), 0);
    if (item) {
        QLineEdit* line = new QLineEdit;
        // for x
        //! I have no idea why I need to reset all properties and even connect if I work with pointer to the item
        line->setText(QString::number(x));
        line->setProperty("row", coordOfDots.size() - 1);
        line->setProperty("column", 0);
        connect(line, SIGNAL(textChanged(const QString&)), this, SLOT(onCoordinateChanged(const QString&)));
        ui->points_list->setCellWidget(coordOfDots.size() - 1, 0, line);
        // for y
        line = new QLineEdit;
        line->setText(QString::number(y));
        line->setProperty("row", coordOfDots.size() - 1);
        line->setProperty("column", 1);
        connect(line, SIGNAL(textChanged(const QString&)), this, SLOT(onCoordinateChanged(const QString&)));
        ui->points_list->setCellWidget(coordOfDots.size() - 1, 1, line);
    } else {
        addPointItem(x, y);
    }
}

void MainWindow::on_button_new_clicked()
{
    ui->plot->setDisabled(false);

    //Clear the plot
	ui->plot->clearItems();
	ui->plot->replot();

	//Clear the list
	ui->points_list->setRowCount(0);

    //Clear the list of points
    coordOfDots.clear();

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
    // remove this point
    coordOfDots.erase(coordOfDots.begin() + row);
    toPlot();
	//Remove this row
	ui->points_list->removeRow(row);
	//Decrement all above button properties for further usages
	for(int i=row;i<ui->points_list->rowCount()-1;i++)
	{
		ui->points_list->cellWidget(i,2)->setProperty("row",ui->points_list->cellWidget(i,2)->property("row").toInt()-1);
	}
}

void MainWindow::onCoordinateChanged(const QString& text)
{
	int row=sender()->property("row").toInt();
	int column=sender()->property("column").toInt();
	std::cout<<"at row "<<row<<" and column "<<column<<" text is "<<text.toStdString()<<std::endl;

    while (coordOfDots.size() < row) {
        coordOfDots.push_back(QPointF(0 , 0));
    }
    if (column == 0) { // if x was changed
        coordOfDots[row].setX(text.toDouble());
    } else if (column == 1) { // if y was changed
        coordOfDots[row].setY(text.toDouble());
    }

    toPlot();
}

void MainWindow::addPointItem(double x, double y)
{
    //Insert new row before plus button
    ui->points_list->insertRow(ui->points_list->rowCount()-1);
    //Add box for x
    QTableWidgetItem *item=new QTableWidgetItem;
    QLineEdit *line=new QLineEdit;
    line->setText(QString::number(x));
    line->setProperty("row",ui->points_list->rowCount()-2);
    line->setProperty("column",0);
    connect(line, SIGNAL(textChanged(const QString&)), this, SLOT(onCoordinateChanged(const QString&)));
    ui->points_list->setItem(ui->points_list->rowCount()-2, 0, item);
    ui->points_list->setCellWidget(ui->points_list->rowCount()-2, 0, line);
    //Add box for y
    item=new QTableWidgetItem;
    line=new QLineEdit;
    line->setText(QString::number(y));
    line->setProperty("row",ui->points_list->rowCount()-2);
    line->setProperty("column",1);
    connect(line, SIGNAL(textChanged(const QString&)), this, SLOT(onCoordinateChanged(const QString&)));
    ui->points_list->setItem(ui->points_list->rowCount()-2, 1, item);
    ui->points_list->setCellWidget(ui->points_list->rowCount()-2, 1, line);
    //Add minus button
    item=new QTableWidgetItem;
    QPushButton *button=new QPushButton;
    button->setProperty("row",ui->points_list->rowCount()-2);
    button->setText("-");
    connect(button, SIGNAL(clicked()), this, SLOT(removePointItem()));
    ui->points_list->setItem(ui->points_list->rowCount()-2, 2, item);
    ui->points_list->setCellWidget(ui->points_list->rowCount()-2, 2, button);
    ui->points_list->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::addToListFromPlot()
{
    // get position of the latest coordinates
    int i = coordOfDots.size();
    // get the current number of rows
    int rows = ui->points_list->rowCount();
    std::cout << "Current number of rows: " << rows << std::endl;
	if (rows < i + 2)
	{
        ui->points_list->insertRow(ui->points_list->rowCount()-1);
        QTableWidgetItem *item = new QTableWidgetItem;
        ui->points_list->setItem(ui->points_list->rowCount()-2, 0, item);
        item=new QTableWidgetItem;
        ui->points_list->setItem(ui->points_list->rowCount()-2, 1, item);

        rows = ui->points_list->rowCount();
    }

    // get the latest coordinates
    double x = coordOfDots[i -1].x();
    double y = coordOfDots[i -1].y();

	addPointItem(x, y);

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
    ui->plot->setDisabled(true);
	//Clear the list
	ui->points_list->clear();
	ui->points_list->setRowCount(0);

    coordOfDots.clear();
}

//! TODO: avoid extra plot (may be it's unnecessary)
void MainWindow::toPlot() {
    ui->plot->clearItems();
    ui->plot->replot();
    // place first point on the plot. There is not enough points to plot lines
    double dotSize=0.08;
    QCPItemEllipse *dot = new QCPItemEllipse(ui->plot);
    ui->plot->addItem(dot);
    dot->topLeft->setCoords(coordOfDots[0].x()-dotSize, coordOfDots[0].y()+dotSize);
    dot->bottomRight->setCoords(coordOfDots[0].x()+dotSize, coordOfDots[0].y()-dotSize);
    // lines and other points
    if(coordOfDots.size() >= 2) {
        for (auto i = coordOfDots.begin() + 1 ; i != coordOfDots.end(); ++i) {
            dot = new QCPItemEllipse(ui->plot);
            ui->plot->addItem(dot);
            dot->topLeft->setCoords((*i).x()-dotSize, (*i).y()+dotSize);
            dot->bottomRight->setCoords((*i).x()+dotSize, (*i).y()-dotSize);

            QCPItemLine *line = new QCPItemLine(ui->plot);
            ui->plot->addItem(line);
            line->start->setCoords(*(i - 1));
            line->end->setCoords(*i);
            line->setPen(QPen(QColor(0, 148, 255)));
        }
    }
    ui->plot->replot();
}
