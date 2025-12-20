#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include "HalconCpp.h"
using namespace  HalconCpp;
class WorkerThread : public QThread
{
    Q_OBJECT

public:
    explicit WorkerThread(QObject *parent = nullptr);
    ~WorkerThread();

    void startWork();      // 启动线程工作
    void stopWork();       // 请求停止线程
    bool isRunningSafely(); // 查询线程是否在安全运行


    void setDispWindow(HalconCpp::HTuple &window);

signals:
    void progress(int value);   // 示例信号：传递进度
    void finished();            // 线程完成时发出
    void stopped();             // 手动停止线程

protected:
    void run() override;

private:


    bool m_stopRequested;

    //创建一个窗口句柄
    HalconCpp::HTuple hv_window;
};

#endif // WORKERTHREAD_H
