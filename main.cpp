#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "systemmonitor.h"

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("systemMonitor", new SystemMonitor());
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
