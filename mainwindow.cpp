#include "mainwindow.h"
#include "model.h"
#include "savedialog.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <stdlib.h>

//class constructors/destructors

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	//Plot stuff
	ui->plot->setInteractions(QCP::iRangeZoom | QCP::iSelectPlottables);
	ui->plot->xAxis->setRange(-10, 10);
	ui->plot->yAxis->setRange(-10, 10);

	clearAll();

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

	// For adding points
	connect(ui->plot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(onPlotClicked()));
	//Clear button
	connect(ui->button_clear_model, SIGNAL(clicked()), this, SLOT(clearAll()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

//class slots

void MainWindow::onPlotClicked()
{
	QPoint p = ui->plot->mapFromGlobal(QCursor::pos());

	//Get click coordinates in plot scale
	double x = ui->plot->xAxis->pixelToCoord(p.x());
	double y = ui->plot->yAxis->pixelToCoord(p.y());
	coordOfDots.push_back(QPointF(x, y));

	//Log coordinates
	std::cout <<"Mouse position on plot: "<< x << "\t" << y << std::endl;

	updatePlot();
	// add to list
	QWidget* item = ui->points_list->cellWidget(coordOfDots.size(), 0);
	if (item)
	{
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
	}
	else
	{
		addPointListItem(x, y);
	}
}

void MainWindow::on_button_new_clicked()
{
	ui->plot->setDisabled(false);

	clearAll();
	ui->plot->setDisabled(false);

	//Add items to list
	//!TODO: See if I need to free this memory(it doesn't work if I do)
	ui->points_list->insertRow(0);
	QTableWidgetItem *item=new QTableWidgetItem;
	//Add plus buttom
	QPushButton *button=new QPushButton;
	button->setText("+");
	connect(button,SIGNAL(clicked()),this,SLOT(addPointListItem()));
	ui->points_list->setItem(0,0,item);
	ui->points_list->setCellWidget(0,0,button);
	ui->points_list->setSpan(0,0,1,3);
	//Add two first dots
	for(int i=0; i<2; i++)
	{
		addPointListItem();
	}
}

void MainWindow::on_button_save_model_clicked()
{
	bool ok;
	QString name = QInputDialog::getText(this, "Save model as", "Model name:", QLineEdit::Normal, "", &ok);
	if(ok && !name.isEmpty())
	{
		std::cout<<"Saving as: "<<name.toStdString()<<std::endl;
		if(getModelNames().contains(name,Qt::CaseInsensitive))
		{
			QMessageBox msgBox;
			msgBox.setText("Model with the same name already exists");
			msgBox.setInformativeText("Do you want to overwrite it?");
			msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
			//msgBox.setDefaultButton(QMessageBox::Ok);
			int res = msgBox.exec();
			switch(res)
			{
				case QMessageBox::Ok:
					for(auto i=models.begin();i<models.end();i++)
					{
						if((*i).getName()==name)
						{
							(*i).setPoints(coordOfDots);
						}
					}
					break;
				case QMessageBox::Cancel:
					break;
				default:
					break;
			}
		}
		else
		{
			models.push_back(Model(name,coordOfDots));
		}
	}
	updateModelList();
}

void MainWindow::on_model_list_activated(const QString &str)
{
	for(auto i=models.begin();i<models.end();i++)
	{
		if((*i).getName()==str)
		{
			coordOfDots=(*i).getPoints();
		}
	}
	updatePlot();
}

void MainWindow::onCoordinateChanged(const QString& text)
{
	int row=sender()->property("row").toInt();
	int column=sender()->property("column").toInt();
	std::cout<<"at row "<<row<<" and column "<<column<<" text is "<<text.toStdString()<<std::endl;

	while (coordOfDots.size() < row)
	{
		coordOfDots.push_back(QPointF(0 , 0));
	}
	if (column == 0)
	{
		// if x was changed
		coordOfDots[row].setX(text.toDouble());
	} else if (column == 1)
	{
		// if y was changed
		coordOfDots[row].setY(text.toDouble());
	}

	updatePlot();
}

void MainWindow::removePointListItem()
{
	//Get sender row
	int row=sender()->property("row").toInt();
	// remove this point
	coordOfDots.erase(coordOfDots.begin() + row);
	updatePlot();
	//Remove this row
	ui->points_list->removeRow(row);
	//Decrement all above button properties for further usages
	for(int i=row;i<ui->points_list->rowCount()-1;i++)
	{
		ui->points_list->cellWidget(i,2)->setProperty("row",ui->points_list->cellWidget(i,2)->property("row").toInt()-1);
	}
}

void MainWindow::addPointListItem(double x, double y)
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
	connect(button, SIGNAL(clicked()), this, SLOT(removePointListItem()));
	ui->points_list->setItem(ui->points_list->rowCount()-2, 2, item);
	ui->points_list->setCellWidget(ui->points_list->rowCount()-2, 2, button);
	ui->points_list->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::addPointListItem()
{
	addPointListItem(0, 0);
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

	addPointListItem(x, y);
}

void MainWindow::clearAll()
{
	//Clear the plot
	ui->plot->clearItems();
	ui->plot->replot();
	ui->plot->setDisabled(true);
	//Clear the list
	ui->points_list->setRowCount(0);
	//Clear points vector
	coordOfDots.clear();
}

//class methods

QStringList MainWindow::getModelNames()
{
	QStringList list;
	for(auto i=models.begin();i<models.end();i++)
	{
		list.append((*i).getName());
	}
	return list;
}

void MainWindow::updateModelList()
{
	ui->model_list->clear();
	ui->model_list->addItems(getModelNames());
}

//! TODO: avoid extra plot (may be it's unnecessary)
void MainWindow::updatePlot()
{
	ui->plot->clearItems();
	ui->plot->replot();
	// place first point on the plot. There is not enough points to plot lines
	double dotSize=0.08;
	QCPItemEllipse *dot = new QCPItemEllipse(ui->plot);
	ui->plot->addItem(dot);
	dot->topLeft->setCoords(coordOfDots[0].x()-dotSize, coordOfDots[0].y()+dotSize);
	dot->bottomRight->setCoords(coordOfDots[0].x()+dotSize, coordOfDots[0].y()-dotSize);
	// lines and other points
	if(coordOfDots.size() >= 2)
	{
		for (auto i = coordOfDots.begin() + 1 ; i != coordOfDots.end(); ++i)
		{
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
