#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "qcustomplot.h"
#include "model.h"


#define PLOT_MODE 0
#define MODEL_MODE 1

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
		void on_button_save_to_file_clicked();
		void on_button_load_from_file_clicked();
		void on_plot_mode_toggled(bool checked);
		void on_model_mode_toggled(bool checked);
		void on_model_list_activated(const QString &str);

		void onCoordinateChanged();
		void onPlotClicked();

		void addPointListItem(double, double);
		void addPointListItem();
		void removePointListItem();
		void clearAll();



	private:
		Ui::MainWindow *ui;
		Plot curPlot;
		QVector<Plot> plots;
		Model curModel;
		QVector<Model> models;
		int mode;
		QStringList getModelNames();
		void updatePlot();
		void updateModelList();
		void updatePointList();
		void updatePointsFromList();
		void setupMode();
};

#endif // MAINWINDOW_H
