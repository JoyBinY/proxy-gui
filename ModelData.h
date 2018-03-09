#ifndef MODELDATA_H
#define MODELDATA_H

#include "UserData.h"
#include <QAbstractListModel>

class ProxyListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    ProxyListModel(const QList<ProxyData> &servers, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE QVariant data(int index, const QString &property) const;
    Q_INVOKABLE bool setData(int index, const QString &value, const QString &property);
    Q_INVOKABLE void addItem(const QString &remarks, const QString &ip, const QString &port, const QString &type,
                             const QString &parentType, const QString &cert = "", const QString &key = "");
    Q_INVOKABLE void removeItem(int index);
    Q_INVOKABLE bool updownItem(int index, int up_down);
    Q_INVOKABLE void reset();
    Q_INVOKABLE void save();

Q_SIGNALS:
    void modelSaved(const QList<ProxyData>&);

private:
    void init_model_data();
    int get_fit_role(const QString &property) const;

private:
    QList<ProxyData> m_servers;
    QVector<QHash<int, QString> > m_modelData;
};

#endif // MODELDATA_H
