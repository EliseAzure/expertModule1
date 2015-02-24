#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "model.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
	void on_button_new_clicked();
	void on_button_save_model_clicked();
	void on_model_list_activated(const QString &str);

	void onCoordinateChanged(const QString & text);
    void onPlotClicked();

	void addPointListItem(double, double);
	void addPointListItem();
    void addToListFromPlot();	
	void removePointListItem();
	void clearAll();

private:
    Ui::MainWindow *ui;
	QVector<QPointF> coordOfDots;
	QVector<Model> models;
	QStringList getModelNames();
	void updatePlot();
	void updateModelList();
};

#endif // MAINWINDOW_H
