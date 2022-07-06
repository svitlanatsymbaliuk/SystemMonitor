#include "systemmonitor.h"

#include <QDebug>

#include <sys/sysinfo.h>

SystemMonitor::SystemMonitor(QObject *parent):QObject(parent){
    m_timer = new QTimer(this);
    m_timer->setInterval((1000));
    connect(m_timer, &QTimer::timeout, this, &SystemMonitor::timeout);

    setTotalPhysicalMemory();
    setUsedPhysicalMemory();

    initCPUUsage();
    setUsedCPU();

    setTotalDiskMemory();
    setUsedDiskMemory();

    setProcessesCount();
    setThreadsCount();

    m_usedPhysicalMemoryValue.setX(m_usedPhysicalMemoryValue.x()+1);
    m_usedPhysicalMemoryValue.setY(m_usedPhysicalMemory);

    m_usedCPUValue.setX(m_usedCPUValue.x()+1);
    m_usedCPUValue.setY(m_usedCPU);

    m_usedDiskMemoryValue.setX(m_usedDiskMemoryValue.x()+1);
    m_usedDiskMemoryValue.setY(m_usedDiskMemory);

    emit noticeTotalPhysicalMemory();
    emit noticeTotalDiskMemory();
    emit usedPhysicalMemoryChanged();
    emit usedCPUChanged();
    emit usedDiskMemoryChanged();
    emit processesCountChanged();
    emit threadsCountChanged();
}

float SystemMonitor::getTotalPhysicalMemory() const {
    return m_totalPhysicalMemory;
}

float SystemMonitor::getUsedPhysicalMemory() const {
    return m_usedPhysicalMemory;
}

QPointF SystemMonitor::usedPhysicalMemoryValue() const{
    return m_usedPhysicalMemoryValue;
}

float SystemMonitor::getUsedCpuValue() const {
    return m_usedCPU;
}

QPointF SystemMonitor::usedCPUValue() const{
    return m_usedCPUValue;
}

float SystemMonitor::getTotalDiskMemory() const {
    return m_totalDiskMemory;
}

float SystemMonitor::getUsedDiskMemory() const {
    return m_usedDiskMemory;
}

QPointF SystemMonitor::usedDiskMemoryValue() const {
    return m_usedDiskMemoryValue;
}

int SystemMonitor::getProcessesCount() const {
    return m_processesCount;
}

int SystemMonitor::getThreadsCount() const {
    return m_threadsCount;
}

void SystemMonitor::start() {
    m_timer->start();
    emit noticeTotalPhysicalMemory();
    emit noticeTotalDiskMemory();
}

void SystemMonitor::stop() {
    m_timer->stop();
}

void SystemMonitor::timeout() {
    setUsedPhysicalMemory();
    setUsedCPU();
    setUsedDiskMemory();
    setProcessesCount();
    setThreadsCount();

    m_usedPhysicalMemoryValue.setX(m_usedPhysicalMemoryValue.x()+1);
    m_usedPhysicalMemoryValue.setY(m_usedPhysicalMemory);

    m_usedCPUValue.setX(m_usedCPUValue.x()+1);
    m_usedCPUValue.setY(m_usedCPU);

    m_usedDiskMemoryValue.setX(m_usedDiskMemoryValue.x()+1);
    m_usedDiskMemoryValue.setY(m_usedDiskMemory);

    emit usedPhysicalMemoryChanged();
    emit usedCPUChanged();
    emit usedDiskMemoryChanged();
    emit processesCountChanged();
    emit threadsCountChanged();
}

unsigned long long SystemMonitor::lastTotalUser = 0;

unsigned long long SystemMonitor::lastTotalUserLow = 0;

unsigned long long SystemMonitor::lastTotalSys = 0;

unsigned long long SystemMonitor::lastTotalIdle = 0;

void SystemMonitor::setTotalPhysicalMemory() {
    struct sysinfo memoryInfo;
    sysinfo(&memoryInfo);
    m_totalPhysicalMemory = (float)(memoryInfo.totalram * memoryInfo.mem_unit)/1000000000;
    qDebug() << "Total memory: " << m_totalPhysicalMemory;
}

void SystemMonitor::setUsedPhysicalMemory() {
    struct sysinfo memoryInfo;
    sysinfo(&memoryInfo);
    m_usedPhysicalMemory = (float)((memoryInfo.totalram - memoryInfo.freeram) * memoryInfo.mem_unit)/1000000000;
    qDebug() << "Used memory: " << m_usedPhysicalMemory;
}

void SystemMonitor::initCPUUsage() {
    FILE* file = fopen("/proc/stat", "r");
    fscanf(file, "cpu %llu %llu %llu %llu", &lastTotalUser, &lastTotalUserLow,
        &lastTotalSys, &lastTotalIdle);
    fclose(file);
}

void SystemMonitor::setUsedCPU() {
    float percent;
    FILE* file;
    unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

    file = fopen("/proc/stat", "r");
    fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow,
        &totalSys, &totalIdle);
    fclose(file);

    if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow ||
        totalSys < lastTotalSys || totalIdle < lastTotalIdle){
        //Overflow detection. Just skip this value.
        percent = -1.0;
    }
    else{
        total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) +
            (totalSys - lastTotalSys);
        percent = total;
        total += (totalIdle - lastTotalIdle);
        percent /= total;
        percent *= 100;
    }

    lastTotalUser = totalUser;
    lastTotalUserLow = totalUserLow;
    lastTotalSys = totalSys;
    lastTotalIdle = totalIdle;

    m_usedCPU = percent;
    qDebug() << "CPU usage" << m_usedCPU;
}

void SystemMonitor::setTotalDiskMemory() {
    foreach(const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {
        if(storage.isValid() && storage.isReady() && !storage.isReadOnly()) {
            m_totalDiskMemory += (float)storage.bytesTotal()/1000/1000/1000;
        }
    }
    qDebug() << "Disks Memory: " << m_totalDiskMemory << "Gb";
}

void SystemMonitor::setUsedDiskMemory() {
    float availableDiskMemory = 0;
    foreach(const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {
        if(storage.isValid() && storage.isReady() && !storage.isReadOnly()) {
            availableDiskMemory += (float)storage.bytesAvailable()/1000/1000/1000;
        }
    }
    m_usedDiskMemory = m_totalDiskMemory - availableDiskMemory;
    qDebug() << "Used disks memory: " << m_usedDiskMemory;
}

void SystemMonitor::setProcessesCount() {
    m_process.start("ps", QStringList() << "-e");
    m_process.waitForFinished();
    QTextStream stream(&m_process);
    m_processesCount = 0;
    while (!stream.atEnd()) {
        stream.readLine();
        ++m_processesCount;
    }
    m_process.close();
    qDebug() << "Processes: " << --m_processesCount;
}

void SystemMonitor::setThreadsCount() {
    m_process.start("ps", QStringList() << "-eo" << "nlwp");
    m_process.waitForFinished();
    QTextStream stream(&m_process);
    m_threadsCount = 0;
    while (!stream.atEnd()) {
        m_threadsCount += stream.readLine().toInt();
    }
    m_process.close();
    qDebug() << "Threads: " << m_threadsCount;
}
