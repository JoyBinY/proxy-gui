#include "ModelData.h"
#include "UserData.h"

ProxyListModel::ProxyListModel(const QList<ProxyData> &servers, QObject *parent) :
    m_servers(servers), QAbstractListModel(parent) {
    init_model_data();
}

int ProxyListModel::rowCount(const QModelIndex &) const {
    return m_modelData.size();
}

QVariant ProxyListModel::data(const QModelIndex &index, int role) const {
    return m_modelData.size() > index.row() ? m_modelData[index.row()][role] : QVariant();
}

QHash<int, QByteArray> ProxyListModel::roleNames() const {
    int role = Qt::UserRole;
    QHash<int, QByteArray> roleNames;
    roleNames.insert(role++, "ip");
    roleNames.insert(role++, "port");
    roleNames.insert(role++, "remarks");
    roleNames.insert(role++, "cert");
    roleNames.insert(role++, "key");
    roleNames.insert(role++, "type");
    roleNames.insert(role++, "parentType");
    return roleNames;
}

QVariant ProxyListModel::data(int index, const QString &property) const {
    if (m_modelData.size() > index && index > -1) {
        int role = get_fit_role(property);
        return m_modelData[index][role];
    } else {
        return QString();
    }
}

bool ProxyListModel::setData(int index, const QString &value, const QString &property) {
    if (m_modelData.size() > index && index > -1) {
        int role = get_fit_role(property);
        m_modelData[index][role] = value;
        return true;
    } else {
        return false;
    }
}

void ProxyListModel::addItem(const QString &remarks, const QString &ip, const QString &port,
                             const QString &type, const QString &parentType, const QString &cert, const QString &key) {
    beginResetModel();
    int role = Qt::UserRole;
    QHash<int, QString> data;
    data.insert(role++, ip);
    data.insert(role++, port);
    data.insert(role++, remarks);
    data.insert(role++, cert);
    data.insert(role++, key);
    data.insert(role++, type);
    data.insert(role++, parentType);
    m_modelData.append(data);
    endResetModel();
}

void ProxyListModel::reset() {
    init_model_data();
}

void ProxyListModel::removeItem(int index) {
    if (m_modelData.size() > index && index > -1) {
        beginResetModel();
        m_modelData.remove(index);
        endResetModel();
    }
}

bool ProxyListModel::updownItem(int index, int up_down) {
    bool ret = false;
    if (m_modelData.size() > index && index > -1) {
        if ((up_down == 1 && index < m_modelData.size() - 1)
                || (up_down == -1 && index > 0)) {
            beginResetModel();
            const auto tmp = m_modelData[index];
            m_modelData[index] = m_modelData[index + up_down];
            m_modelData[index + up_down] = tmp;
            endResetModel();
            ret = true;
        }
    }
    return ret;
}

void ProxyListModel::save() {
    m_servers.clear();
    foreach (const auto &tmp, m_modelData) {
        int role = Qt::UserRole;
        ProxyData data;
        data.setIP(tmp[role++]);
        data.setPort(tmp[role++]);
        data.setRemarks(tmp[role++]);
        data.setCert(tmp[role++]);
        data.setKey(tmp[role++]);
        data.setType(tmp[role++]);
        data.setParentType(tmp[role++]);
        m_servers.append(data);
    }
    init_model_data();
    emit modelSaved(m_servers);
}

int ProxyListModel::get_fit_role(const QString &property) const {
    int role = Qt::UserRole;
    if ("ip" == property) { }
    else if ("port" == property) { role += 1; }
    else if ("remarks" == property) { role += 2; }
    else if ("cert" == property) { role += 3; }
    else if ("key" == property) { role += 4; }
    else if ("type" == property) { role += 5; }
    else if ("parentType" == property) { role += 6; }
    return role;
}

void ProxyListModel::init_model_data() {
    m_modelData.clear();
    beginResetModel();
    foreach (const auto &tmp, m_servers) {
        int role = Qt::UserRole;
        QHash<int, QString> data;
        data.insert(role++, tmp.ip());
        data.insert(role++, tmp.port());
        data.insert(role++, tmp.remarks());
        data.insert(role++, tmp.cert());
        data.insert(role++, tmp.key());
        data.insert(role++, tmp.type());
        data.insert(role++, tmp.parentType());
        m_modelData.append(data);
    }
    endResetModel();
}
