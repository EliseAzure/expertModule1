#include "mainwindow.h"
#include "model.h"
#include "saveload.h"

#include <iostream>
#include <stdlib.h>
#include <QDomDocument>

//class constructors/destructors

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	setupMode(PLOT_MODE);

	//MODEL test
	points.push_back(QPointF(0,0));
	points.push_back(QPointF(0,1));
	points.push_back(QPointF(2,2));
	points.push_back(QPointF(6,7));
	Plot plot("plotname",points);
	Term term("heat",plot,0,6);
	Model model("model1",QVector<Term>{term},0,15);


	//Plot clicks handler
	connect(ui->plot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(onPlotClicked()));
	//Clear button handler(also used in many methods as standart cleaning method)
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
	points.push_back(QPointF(x, y));

	//Log coordinates
	std::cout <<"Mouse position on plot: "<< x << "\t" << y << std::endl;

	updatePlot();
	updatePointList();
}

void MainWindow::on_button_new_clicked()
{
	//Full clear and enabling the plot
	clearAll();
	ui->plot->setDisabled(false);

	//Add items to list
	//!TODO: See if I need to free this memory(it doesn't work if I do)
	ui->point_list->insertRow(0);
	QTableWidgetItem *item=new QTableWidgetItem;
	//Add plus buttom
	QPushButton *button=new QPushButton;
	button->setText("+");
	connect(button,SIGNAL(clicked()),this,SLOT(addPointListItem()));
	ui->point_list->setItem(0,0,item);
	ui->point_list->setCellWidget(0,0,button);
	ui->point_list->setSpan(0,0,1,3);
	//Add two first dots
	for(int i=0; i<2; i++)
	{
		addPointListItem();
	}
}

void MainWindow::on_button_save_model_clicked()
{
	bool ok;
	//Input model name dialog
	QString name = QInputDialog::getText(this, "Save model as", "Model name:", QLineEdit::Normal, "", &ok);
	if(ok && !name.isEmpty())
	{
		std::cout<<"Saving as: "<<name.toStdString()<<std::endl;
		//See if we already have a model with the same name
		if(getModelNames().contains(name,Qt::CaseInsensitive))
		{
			//Go for a message box with overwriting shit
			QMessageBox msgBox;
			msgBox.setText("Model with the same name already exists");
			msgBox.setInformativeText("Do you want to overwrite it?");
			msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
			int res = msgBox.exec();
			//See what we got from the message box
			switch(res)
			{
				case QMessageBox::Ok:
					//Go to the models list and find our model
					for(auto i=models.begin();i<models.end();i++)
					{
						if((*i).getName()==name)
						{
							//Replace it's points with current
							(*i).setPoints(points);
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
			//If we do not then just add a new model to out models list
			models.push_back(Plot(name,points));
		}
	}
	updateModelList();
}

void MainWindow::on_button_save_to_file_clicked()
{
	QString saveDir = QFileDialog::getSaveFileName(this, tr("Save File"), "/home/models.mdl", tr("Models (*.mdl)"));
	SaveLoad::save(models, saveDir);
}

void MainWindow::on_button_load_from_file_clicked()
{
	QString openDir = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/models.mdl", tr("Models (*.mdl)"));
	SaveLoad::load(&models, openDir);
	qDebug()<<openDir;
	if(models.length()>0)
	{
		points=models[0].getPoints();
		qDebug()<<QString::number(models.length());
		qDebug()<<QString::number(points.length());
		updateModelList();
		ui->model_list->setCurrentIndex(0);
		updatePlot();
		updatePointList();
	}
}

void MainWindow::on_model_list_activated(const QString &str)
{
	//We are gonna find a model with a name that was selected in the combo box
	for(auto i=models.begin();i<models.end();i++)
	{
		if((*i).getName()==str)
		{
			//And copy it's points to current points
			points=(*i).getPoints();
		}
	}
	updatePlot();
	updatePointList();
}

void MainWindow::onCoordinateChanged()
{
	//Just update points and plot
	updatePointsFromList();
	updatePlot();
}

void MainWindow::removePointListItem()
{
	//Get sender row
	int row=sender()->property("row").toInt();
	// remove this point
	points.erase(points.begin() + row);
	updatePlot();
	//Remove this row
	ui->point_list->removeRow(row);
	//Decrement all above button properties for further usages
	for(int i=row;i<ui->point_list->rowCount()-1;i++)
	{
		ui->point_list->cellWidget(i,2)->setProperty("row",ui->point_list->cellWidget(i,2)->property("row").toInt()-1);
	}
}

void MainWindow::addPointListItem(double x, double y)
{
	//Insert new row before plus button
	ui->point_list->insertRow(ui->point_list->rowCount()-1);
	//Add box for x
	QTableWidgetItem *item=new QTableWidgetItem;
	QLineEdit *line=new QLineEdit;
	line->setText(QString::number(x));
	connect(line, SIGNAL(textEdited(const QString&)), this, SLOT(onCoordinateChanged()));
	ui->point_list->setItem(ui->point_list->rowCount()-2, 0, item);
	ui->point_list->setCellWidget(ui->point_list->rowCount()-2, 0, line);
	//Add box for y
	item=new QTableWidgetItem;
	line=new QLineEdit;
	line->setText(QString::number(y));
	connect(line, SIGNAL(textEdited(const QString&)), this, SLOT(onCoordinateChanged()));
	ui->point_list->setItem(ui->point_list->rowCount()-2, 1, item);
	ui->point_list->setCellWidget(ui->point_list->rowCount()-2, 1, line);
	//Add minus button
	item=new QTableWidgetItem;
	QPushButton *button=new QPushButton;
	button->setProperty("row",ui->point_list->rowCount()-2);
	button->setText("-");
	connect(button, SIGNAL(clicked()), this, SLOT(removePointListItem()));
	ui->point_list->setItem(ui->point_list->rowCount()-2, 2, item);
	ui->point_list->setCellWidget(ui->point_list->rowCount()-2, 2, button);
	ui->point_list->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::addPointListItem()
{
	addPointListItem(0, 0);
}

void MainWindow::clearAll()
{
	//Clear and disable the plot
	ui->plot->clearItems();
	ui->plot->replot();
	ui->plot->setDisabled(true);
	//Clear the list
	ui->point_list->setRowCount(0);
	//Clear points vector
	points.clear();
}

//class methods

QStringList MainWindow::getModelNames()
{
	//Returns all names from models list(sorted)
	QStringList list;
	for(auto i=models.begin();i<models.end();i++)
	{
		list.append((*i).getName());
	}
	list.sort();
	return list;
}

void MainWindow::updateModelList()
{
	//Puts all names from models list to our combo box(sorted)
	ui->model_list->clear();
	ui->model_list->addItems(getModelNames());
}

void MainWindow::updatePointList()
{	
	//Get row count(it's changed if I remove rows in process, so we need to get it before all)
	int lastRowCount=ui->point_list->rowCount();
	//See if it's 0
	if(!lastRowCount)
	{
		//!TODO: make new method for this and a signal from the button
		//Add plus buttom
		ui->point_list->insertRow(0);
		QTableWidgetItem *item=new QTableWidgetItem;
		QPushButton *button=new QPushButton;
		button->setText("+");
		connect(button,SIGNAL(clicked()),this,SLOT(addPointListItem()));
		ui->point_list->setItem(0,0,item);
		ui->point_list->setCellWidget(0,0,button);
		ui->point_list->setSpan(0,0,1,3);
		lastRowCount=ui->point_list->rowCount();
	}
	//Remove redundant rows from list
	for(int i=0;i<lastRowCount-1-points.size();i++)
	{
		ui->point_list->removeRow(lastRowCount-i-2);
	}
	//Or add some
	for(int i=0;i<points.size()-lastRowCount+1;i++)
	{
		addPointListItem();
	}
	//Replace all coordinates with new
	for(int i=0;i<points.size();i++)
	{
		((QLineEdit*)ui->point_list->cellWidget(i,0))->setText(QString::number(points[i].x()));
		((QLineEdit*)ui->point_list->cellWidget(i,1))->setText(QString::number(points[i].y()));
	}
}

void MainWindow::updatePointsFromList()
{
	//Clear the list so we can push_back to it
	points.clear();
	//Pushing coordinates from list to our points vector
	for(int i=0;i<ui->point_list->rowCount()-1;i++)
	{
		points.push_back(QPointF(((QLineEdit*)ui->point_list->cellWidget(i,0))->text().toDouble(),((QLineEdit*)ui->point_list->cellWidget(i,1))->text().toDouble()));
	}
}

void MainWindow::updatePlot()
{
	//! TODO: avoid extra plot (may be it's unnecessary)
	//Clear and update the plot
	ui->plot->clearItems();
	ui->plot->replot();
	// place first point on the plot. There is not enough points to plot lines
	double dotSize=0.08;
	QCPItemEllipse *dot = new QCPItemEllipse(ui->plot);
	ui->plot->addItem(dot);
	//! TODO: check for null length of points vector here
	dot->topLeft->setCoords(points[0].x()-dotSize, points[0].y()+dotSize);
	dot->bottomRight->setCoords(points[0].x()+dotSize, points[0].y()-dotSize);
	//Plot lines and other points
	if(points.size() >= 2)
	{
		for (auto i = points.begin() + 1 ; i != points.end(); ++i)
		{
			//Point stuff
			dot = new QCPItemEllipse(ui->plot);
			ui->plot->addItem(dot);
			dot->topLeft->setCoords((*i).x()-dotSize, (*i).y()+dotSize);
			dot->bottomRight->setCoords((*i).x()+dotSize, (*i).y()-dotSize);
			//Line stuff
			QCPItemLine *line = new QCPItemLine(ui->plot);
			ui->plot->addItem(line);
			line->start->setCoords(*(i - 1));
			line->end->setCoords(*i);
			line->setPen(QPen(QColor(0, 148, 255)));
		}
	}
	ui->plot->replot();
}

void MainWindow::setupMode(int mode)
{
	if(mode==PLOT_MODE)
	{
		ui->plot_mode->setChecked(true);
		ui->model_mode->setChecked(false);

		//Plot stuff
		ui->plot->setInteractions(QCP::iRangeZoom | QCP::iSelectPlottables);
		ui->plot->xAxis->setRange(-10, 10);
		ui->plot->yAxis->setRange(-10, 10);

		clearAll();

		//PointList stuff
		//Adjust column sizes(shitcoded)
		//!TODO: replace with accurate methods
		ui->point_list->setColumnCount(3);
		ui->point_list->setColumnWidth(0,ui->point_list->width()*0.4);
		ui->point_list->setColumnWidth(1,ui->point_list->width()*0.4);
		ui->point_list->horizontalHeader()->setStretchLastSection(true);
		//Edit and remove headers
		ui->point_list->setHorizontalHeaderItem(0,new QTableWidgetItem("x"));
		ui->point_list->setHorizontalHeaderItem(1,new QTableWidgetItem("y"));
		ui->point_list->setHorizontalHeaderItem(2,new QTableWidgetItem(""));
		ui->point_list->verticalHeader()->setVisible(false);
		//~PointList stuff
	}
	else if(mode==MODEL_MODE)
	{

	}
}

void MainWindow::on_plot_mode_toggled(bool checked)
{
	if(checked)
	{
		emit setupMode(PLOT_MODE);
	}
}

void MainWindow::on_model_mode_toggled(bool checked)
{
	if(checked)
	{
		emit setupMode(MODEL_MODE);
	}
}
