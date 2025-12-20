#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "HalconCpp.h"
#include "WorkerThread.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:
    void onStartClicked();
    void onStopClicked();
    void onProgress(int value);
    void onFinished();
    void onStopped();


private:
    Ui::MainWindow *ui;


       WorkerThread *worker;


};
#endif // MAINWINDOW_H
