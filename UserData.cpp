#include "UserData.h"
#include <QJsonArray>

void ProxyData::read(const QJsonObject &json) {
    if (json.contains("server") && json["server"].isString()) {
        m_ip = json["server"].toString();
    }
    if (json.contains("server_port") && json["server_port"].isString()) {
        m_port = json["server_port"].toString();
    }
    if (json.contains("cert") && json["cert"].isString()) {
        m_cert = json["cert"].toString();
    }
    if (json.contains("key") && json["key"].isString()) {
        m_key = json["key"].toString();
    }
    if (json.contains("type") && json["type"].isString()) {
        m_type = json["type"].toString();
    }
    if (json.contains("parentType") && json["parentType"].isString()) {
        m_parent_type = json["parentType"].toString();
    }
    if (json.contains("remarks") && json["remarks"].isString()) {
        m_remarks = json["remarks"].toString();
    }
}

void ProxyData::write(QJsonObject &json) {
    json["server"] = m_ip;
    json["server_port"] = m_port;
    json["cert"] = m_cert;
    json["key"] = m_key;
    json["type"] = m_type;
    json["parentType"] = m_parent_type;
    json["remarks"] = m_remarks;
}

////////////////////////////////////////////////////////////////////////

ConfigureData::ConfigureData() {
    m_enabled   = false;
    m_index     = -1;
    m_localPort = 8686;
}

void ConfigureData::read(const QJsonObject &json) {
    if (json.contains("enabled") && json["enabled"].isBool()) {
        m_enabled = json["enabled"].toBool();
    }
    if (json.contains("index") && json["index"].isDouble()) {
        m_index = json["index"].toInt();
    }
    if (json.contains("localPort") && json["localPort"].isDouble()) {
        m_localPort = json["localPort"].toInt();
    }
    if (json.contains("configs") && json["configs"].isArray()) {
        QJsonArray servers = json["configs"].toArray();
        foreach (const auto tmp, servers) {
            ProxyData server;
            server.read(tmp.toObject());
            m_servers.append(server);
        }
    }
}

void ConfigureData::write(QJsonObject &json) {
    json["enabled"] = m_enabled;
    json["index"] = m_index;
    json["localPort"] = m_localPort;

    QJsonArray servers;
    foreach (auto tmp, m_servers) {
        QJsonObject server;
        tmp.write(server);
        servers.append(server);
    }
    json["configs"] = servers;
}
