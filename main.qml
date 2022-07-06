import QtQuick 2.8
import QtCharts 2.2
import QtQuick.Window 2.2
import QtQuick.Controls 2.15

Window {
    id: mainWindow
    visible: true
    width: 640
    height: 480
    title: qsTr("System Monitor")

    Connections {
        target: systemMonitor
        onNoticeTotalPhysicalMemory: {
            memoryUsageAxisY.max = systemMonitor.getTotalPhysicalMemory
        }
        onNoticeTotalDiskMemory: {
            diskMemoryAxisY.max = systemMonitor.getTotalDiskMemory
        }

        onUsedPhysicalMemoryChanged: {
            if (memoryUsageLineSeries.count > 10)
                memoryUsageLineSeries.remove(0)
            memoryUsageLineSeries.append(
                        systemMonitor.usedPhysicalMemoryValue.x,
                        systemMonitor.usedPhysicalMemoryValue.y)
            memoryUsageAxisX.min = memoryUsageLineSeries.at(0).x
            memoryUsageAxisX.max = memoryUsageLineSeries.at(
                        memoryUsageLineSeries.count - 1).x
        }

        onUsedCPUChanged: {
            if (cpuUsageLineSeries.count > 10)
                cpuUsageLineSeries.remove(0)
            cpuUsageLineSeries.append(systemMonitor.usedCPUValue.x,
                                      systemMonitor.usedCPUValue.y)
            cpuUsageAxisX.min = cpuUsageLineSeries.at(0).x
            cpuUsageAxisX.max = cpuUsageLineSeries.at(
                        cpuUsageLineSeries.count - 1).x
        }

        onUsedDiskMemoryChanged: {
            if (diskMemoryLineSeries.count > 10)
                diskMemoryLineSeries.remove(0)
            diskMemoryLineSeries.append(systemMonitor.usedDiskMemoryValue.x,
                                        systemMonitor.usedDiskMemoryValue.y)
            diskMemoryAxisX.min = diskMemoryLineSeries.at(0).x
            diskMemoryAxisX.max = diskMemoryLineSeries.at(
                        diskMemoryLineSeries.count - 1).x
        }

        onProcessesCountChanged: {
            processesText.text = systemMonitor.getProcessesCount
        }

        onThreadsCountChanged: {
            threadsText.text = systemMonitor.getThreadsCount
        }
    }

    ScrollView {
        anchors.fill: parent

        Column {
            id: column
            width: parent.width
            topPadding: 15
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 5

            Row {

                id: buttonsRow
                width: 200
                height: 20
                spacing: 15
                anchors.horizontalCenter: parent.horizontalCenter

                Button {
                    id: startButton
                    text: qsTr("Start")
                    onClicked: {
                        systemMonitor.start()
                    }
                }

                Button {
                    id: stopButton
                    text: qsTr("Stop")
                    onClicked: {
                        systemMonitor.stop()
                    }
                }
            }

            Row {

                id: processesAndThreads
                width: 200
                height: 20
                spacing: 15
                topPadding: 20
                anchors.horizontalCenter: parent.horizontalCenter

                Label {
                    id: processesLabel
                    text: "Processes: "
                }

                Text {
                    id: processesText
                }

                Label {
                    id: threadsLabes
                    text: "Threads: "
                }

                Text {
                    id: threadsText
                }
            }

            ChartView {
                id: memoryUsageChartView
                width: mainWindow.width
                height: 200
                animationOptions: ChartView.NoAnimation
                antialiasing: true

                ValueAxis {
                    id: memoryUsageAxisY
                    min: 0
                    max: 100
                    gridVisible: true
                    labelFormat: "%.0f"
                }

                ValueAxis {
                    id: memoryUsageAxisX
                    min: 0
                    max: 50
                    gridVisible: true
                    labelFormat: " "
                    tickCount: 10
                }

                LineSeries {
                    id: memoryUsageLineSeries
                    name: "Memory usage, Gb/10 sec"
                    axisX: memoryUsageAxisX
                    axisY: memoryUsageAxisY
                }
            }

            ChartView {
                id: cpuUsageChartView
                width: mainWindow.width
                height: 200
                animationOptions: ChartView.NoAnimation
                antialiasing: true

                ValueAxis {
                    id: cpuUsageAxisY
                    min: 0
                    max: 100
                    gridVisible: true
                    labelFormat: "%.0f"
                }

                ValueAxis {
                    id: cpuUsageAxisX
                    min: 0
                    max: 50
                    gridVisible: true
                    labelFormat: " "
                    tickCount: 10
                }

                LineSeries {
                    id: cpuUsageLineSeries
                    name: "CPU usage, % in 10 sec"
                    axisX: cpuUsageAxisX
                    axisY: cpuUsageAxisY
                }
            }
            ChartView {
                id: diskMemoryChartView
                width: mainWindow.width
                height: 200
                animationOptions: ChartView.NoAnimation
                antialiasing: true

                ValueAxis {
                    id: diskMemoryAxisY
                    min: 0
                    max: 100
                    gridVisible: false
                    labelFormat: "%.0f"
                }

                ValueAxis {
                    id: diskMemoryAxisX
                    min: 0
                    max: 50
                    gridVisible: false
                    labelFormat: " "
                    tickCount: 10
                }

                LineSeries {
                    id: diskMemoryLineSeries
                    name: "Mounted disks memory, Gb/10 sec"
                    axisX: diskMemoryAxisX
                    axisY: diskMemoryAxisY
                }
            }
        }
    }
}
