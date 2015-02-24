#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void mouseClickEvent();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_button_new_clicked();
    void on_button_clear_item_clicked();

    void onPlotClicked();
    void addPointItem(double, double);
    void addPointItem();
    void addToListFromPlot();
    // void addToPlotFromList(QTableWidgetItem* item);

    void removePointItem();


private:
    Ui::MainWindow *ui;
    std::vector<QPointF> coordOfDots;
};

#endif // MAINWINDOW_H
