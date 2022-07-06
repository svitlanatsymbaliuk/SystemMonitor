#ifndef SYSTEMMONITOR_H
#define SYSTEMMONITOR_H

#include <QObject>
#include <QPointF>
#include <QTimer>
#include <QStorageInfo>
#include <QProcess>
#include <QString>
#include <QTextStream>

class SystemMonitor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPointF usedPhysicalMemoryValue READ usedPhysicalMemoryValue NOTIFY usedPhysicalMemoryChanged)
    Q_PROPERTY(QPointF usedCPUValue READ usedCPUValue NOTIFY usedCPUChanged)
    Q_PROPERTY(QPointF usedDiskMemoryValue READ usedDiskMemoryValue NOTIFY usedDiskMemoryChanged)
    Q_PROPERTY(int getProcessesCount READ getProcessesCount NOTIFY processesCountChanged)
    Q_PROPERTY(int getThreadsCount READ getThreadsCount NOTIFY threadsCountChanged)
    Q_PROPERTY(float getTotalPhysicalMemory READ getTotalPhysicalMemory NOTIFY noticeTotalPhysicalMemory)
    Q_PROPERTY(float getTotalDiskMemory READ getTotalDiskMemory NOTIFY noticeTotalDiskMemory)

public:
    SystemMonitor(QObject *parent=Q_NULLPTR);

    float getTotalPhysicalMemory() const;
    float getUsedPhysicalMemory() const;
    QPointF usedPhysicalMemoryValue() const;

    float getUsedCpuValue() const;
    QPointF usedCPUValue() const;

    float getTotalDiskMemory() const;
    float getUsedDiskMemory() const;
    QPointF usedDiskMemoryValue() const;

    int getProcessesCount() const;
    int getThreadsCount() const;

signals:
    void usedPhysicalMemoryChanged();
    void usedCPUChanged();
    void usedDiskMemoryChanged();
    void processesCountChanged();
    void threadsCountChanged();

    void noticeTotalPhysicalMemory();
    void noticeTotalDiskMemory();

public slots:
    void start();
    void stop();

private slots:
    void timeout();

private:
    QTimer * m_timer;

    float m_totalPhysicalMemory;
    float m_usedPhysicalMemory;
    QPointF m_usedPhysicalMemoryValue;

    float m_usedCPU;
    QPointF m_usedCPUValue;

    float m_totalDiskMemory;
    float m_usedDiskMemory;
    QPointF m_usedDiskMemoryValue;

    QProcess m_process;
    int m_processesCount;
    int m_threadsCount;

    static unsigned long long lastTotalUser;
    static unsigned long long lastTotalUserLow;
    static unsigned long long lastTotalSys;
    static unsigned long long lastTotalIdle;

    void setTotalPhysicalMemory();
    void setUsedPhysicalMemory();

    void initCPUUsage();
    void setUsedCPU();

    void setTotalDiskMemory();
    void setUsedDiskMemory();

    void setProcessesCount();
    void setThreadsCount();
};

#endif // SYSTEMMONITOR_H
