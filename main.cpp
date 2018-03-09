#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include <QDir>

#include "Configure.h"
#include "ModelData.h"
#include "Service.h"
#include "Logger.h"

#ifndef QT_DEBUG
#define WIN_LOG
#include <windows.h>
#endif

#ifdef WIN_LOG
LONG applicationCrashHandler(EXCEPTION_POINTERS *pException)
{
    EXCEPTION_RECORD* record = pException->ExceptionRecord;
    QString errCode(QString::number(record->ExceptionCode, 16));
    QString errAdr(QString::number((uint)record->ExceptionAddress, 16));

    qFatal(QString("error code:%1 error address:%2").arg(errCode).arg(errAdr).toUtf8().data());

    return Logger::releaseLogger() ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH;
}
#endif

void ready_to_start() {
    QString gui_path = QGuiApplication::applicationDirPath();
    QString proxy_path = gui_path + "/..";
    QDir::setCurrent(proxy_path);

#ifdef WIN_LOG
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)applicationCrashHandler);
    Logger::initLogger();
    qInstallMessageHandler(Logger::addLogRecord);
#endif
}

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    app.setWindowIcon(QIcon("qrc:/images/logo.ico"));

    ready_to_start();
//    qmlRegisterType<Configure>("proxy.ui.configure", 1, 0, "Configure");
//    qmlRegisterType<Service>("proxy.ui.service", 1, 0, "Service");

    qRegisterMetaType<QList<ProxyData> >("QList<ProxyData>");

    ConfigureData data;
    Configure conf(&data);
    Service service(&data);
    ProxyListModel model(data.servers());

    QObject::connect(&model, ProxyListModel::modelSaved, &conf, Configure::onModelSaved);
    QObject::connect(&conf, Configure::enableChanged, &service, Service::onConnectChanged);

    QQmlContext *context = engine.rootContext();
    context->setContextProperty("configure", &conf);
    context->setContextProperty("service", &service);
    context->setContextProperty("listModel", &model);

    engine.load(QUrl(QStringLiteral("qrc:/SystemTray.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    service.startWork();
    bool ret = app.exec();

#ifdef WIN_LOG
    Logger::releaseLogger();
#endif

    return ret;
}
