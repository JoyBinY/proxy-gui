#include <QFile>
#include "Configure.h"

Configure::Configure(ConfigureData *data, QObject *parent)
    : m_data(data), QObject(parent), CFG_FILENAME("gui-config.json")
{
    init_cfg_file();
}

bool Configure::enable() {
    return m_data->enabled();
}

void Configure::setEnable(bool enable) {
    if (enable != m_data->enabled()) {
        m_data->setEnabled(enable);
        write_cfg(m_data);
        emit enableChanged();
    }
}

int Configure::index() {
    return m_data->index();
}

void Configure::setIndex(int index) {
    int pre = m_data->index();
    m_data->setIndex(index);
    emit indexChanged();
    if (index != pre) {
        write_cfg(m_data);
        emit enableChanged();
    }
}

int Configure::port() {
    return m_data->localPort();
}

void Configure::setPort(int port) {
    if (port != m_data->localPort()) {
        m_data->setLocalPort(port);
        emit portChanged();
    }
}

void Configure::applyConfigure() {
    write_cfg(m_data);
}

void Configure::onModelSaved(const QList<ProxyData> &servers) {
    m_data->setServers(servers);
    write_cfg(m_data);
}

void Configure::init_cfg_file() {
    QFile file(CFG_FILENAME);
    if (!file.exists()) {
        write_cfg(m_data);
    } else {
        read_cfg(m_data);
    }
}

void Configure::write_cfg(ConfigureData *cfg) {
    QFile file(CFG_FILENAME);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonObject data;
        cfg->write(data);
        QJsonDocument saveDoc(data);
        file.write(saveDoc.toJson());
        file.close();
    }
}

void Configure::read_cfg(ConfigureData *cfg) {
    QFile file(CFG_FILENAME);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(data));
        cfg->read(loadDoc.object());
        file.close();
    }
}
