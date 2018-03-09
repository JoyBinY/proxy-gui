#include <QSettings>
#include <QGuiApplication>
#include <QDebug>
#include "Service.h"
#include "Configure.h"

Service::Service(ConfigureData *data, QObject *parent) : QObject(parent), m_data(data) {
    connect(&m_process, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(on_state_changed(QProcess::ProcessState)));
    connect(&m_process, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(on_error_occurred(QProcess::ProcessError)));
    connect(&m_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(on_finished(int, QProcess::ExitStatus)));
}

void Service::stop() {
    if (m_process.state() != QProcess::NotRunning) {
        m_process.close();
        enable_ie_proxy(false);
    }
}

bool Service::startUp() {
    QSettings settings(REG_RUN, QSettings::NativeFormat);
    QString name = QGuiApplication::applicationName();
    if (settings.childKeys().contains(name)) {
        QString path = QGuiApplication::applicationFilePath();
        return settings.value(name).toString() == path.replace("/", "\\");
    } else {
        return false;
    }
}

void Service::setStartUp(bool should) {
    QSettings settings(REG_RUN, QSettings::NativeFormat);
    QString name = QGuiApplication::applicationName();
    if (should) {
        QString path = QGuiApplication::applicationFilePath();
        settings.setValue(name, path.replace("/", "\\"));
    } else {
        settings.remove(name);
    }
}

void Service::startWork() {
    if (m_data->enabled()) {
        m_process.start(get_command());
        enable_ie_proxy(true);
    }
}

QString Service::tooltip() {
    return QString("proxy-go 4.3\n%1").arg(m_msg);
}

void Service::setTooltip(const QString &msg) {
    if (m_msg != msg) {
        m_msg = msg;
        emit tooltipChanged();
    }
}

void Service::onConnectChanged() {
    if (m_process.state() != QProcess::NotRunning) {
        m_process.close();
    }
    bool enable = m_data->enabled();
    enable_ie_proxy(enable);
    if (enable) {
        m_process.start(get_command());
    }
}

QString Service::get_command() {
    QString args;
    if (m_data) {
        int index = m_data->index();
        if (m_data->servers().size() > index && index >= 0) {
            const auto &tmp = m_data->servers().at(index);
            args += QString()
                    + "proxy"
                    + " http"
                    + " -t " + "tcp"
                    + " -p " + QString("\":%1\"").arg(m_data->localPort())
                    + " -T " + "tls"
                    + " -P " + QString("\"%1:%2\"").arg(tmp.ip()).arg(tmp.port())
                    + " -C " + tmp.cert()
                    + " -K " + tmp.key();
        }
    }
    qInfo() << args;
    return args;
}

void Service::enable_ie_proxy(bool enable) {
    QSettings settings(REG_IE_SETTING, QSettings::NativeFormat);
    if (enable) {
        settings.setValue("ProxyEnable", 1);
        settings.setValue("ProxyServer", QString("127.0.0.1:%1").arg(m_data->localPort()));
    } else {
        settings.setValue("ProxyEnable", 0);
        settings.remove("ProxyServer");
    }
}

///////////////////////////////////////////////////////////////////

void Service::on_state_changed(QProcess::ProcessState newState) {
    QString msg;
    switch (newState) {
    case QProcess::NotRunning:
        msg = tr("Not running");
        break;
    case QProcess::Starting:
        msg = tr("starting");
        break;
    case QProcess::Running:
        msg = tr("running");
        break;
    default: break;
    }
    setTooltip(msg);
    qDebug() << msg;
}

void Service::on_error_occurred(QProcess::ProcessError error) {
    switch (error) {
    case QProcess::FailedToStart: break;
    case QProcess::Crashed: break;
    case QProcess::Timedout: break;
    case QProcess::WriteError: break;
    case QProcess::ReadError: break;
    case QProcess::UnknownError: break;
    default: break;
    }
}

void Service::on_finished(int exitCode, QProcess::ExitStatus exitStatus) {
    qDebug() << "exit code:" << exitCode;
    if (QProcess::CrashExit == exitStatus)
        qCritical() << "error message:" << m_process.readAllStandardError();
}

QString Service::REG_RUN = "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
QString Service::REG_IE_SETTING = "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Internet Settings";
