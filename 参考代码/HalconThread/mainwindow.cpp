#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "HalconCpp.h"
#include <QDebug>
using namespace HalconCpp;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);





    worker=new WorkerThread(this);

    connect(worker, &WorkerThread::progress, this, &MainWindow::onProgress);
    connect(worker, &WorkerThread::finished, this, &MainWindow::onFinished);
    connect(worker, &WorkerThread::stopped, this, &MainWindow::onStopped);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::onStopClicked);


    HalconCpp::HTuple hv_window;
    //设置窗口属性
    HalconCpp::OpenWindow(0, 0, 1000, 800, static_cast<Hlong>(ui->graphicsView->winId()), "", "",&hv_window);
    HalconCpp::SetColored(hv_window,12);
    HalconCpp::SetHcppInterfaceStringEncodingIsUtf8(true);
    HalconCpp::SetLineWidth(hv_window,20);
    worker->setDispWindow(hv_window);

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::onStartClicked()
{
    qDebug() << "Start button clicked.";
    worker->startWork();
}

void MainWindow::onStopClicked()
{
    qDebug() << "Stop button clicked.";
    worker->stopWork();
}

void MainWindow::onProgress(int value)
{
    ui->progressBar->setValue(value);
    qDebug() << "Progress:" << value;
}

void MainWindow::onFinished()
{
    qDebug() << "Thread finished.";
}

void MainWindow::onStopped()
{
    qDebug() << "Thread was stopped manually.";
}

