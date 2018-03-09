#ifndef USERDATA_H
#define USERDATA_H

#include <QList>
#include <QJsonObject>
#include <QJsonDocument>

class ProType {
public:
    enum Type {
        tls,
        tcp,
        kcp,
        ssh
    };
};

class ProxyData {
public:
    void read(const QJsonObject &json);
    void write(QJsonObject &json);

public:
    QString ip() const { return m_ip; }
    void setIP(const QString &ip) { m_ip = ip; }

    QString port() const { return m_port; }
    void setPort(const QString &port)  { m_port = port; }

    QString remarks() const { return m_remarks; }
    void setRemarks(const QString &remarks)  { m_remarks = remarks; }

    QString cert() const { return m_cert; }
    void setCert(const QString &cert) { m_cert = cert; }

    QString key() const { return m_key; }
    void setKey(const QString &key) { m_key = key; }

    QString type() const { return m_type; }
    void setType(const QString &type) { m_type = type; }

    QString parentType() const { return m_parent_type; }
    void setParentType(const QString &type) { m_parent_type = type; }

private:
    QString m_ip;
    QString m_port;
    QString m_remarks;
    QString m_cert;
    QString m_key;
    QString m_type;
    QString m_parent_type;
};

////////////////////////////////////////////////////////////////////////

class ConfigureData {
public:
    ConfigureData();
    void read(const QJsonObject &json);
    void write(QJsonObject &json);

public:
    bool enabled() const { return m_enabled; }
    int index() const { return m_index; }
    int localPort() const { return m_localPort; }
    QList<ProxyData> servers() { return m_servers; }

    void setEnabled(bool enabled) { m_enabled = enabled; }
    void setIndex(int index)  { m_index = index; }
    void setLocalPort(int port)  { m_localPort = port; }
    void setServers(QList<ProxyData> servers)  { m_servers = servers; }

private:
    bool    m_enabled;
    int     m_index;
    int     m_localPort;
    QList<ProxyData> m_servers;
};

#endif // USERDATA_H
