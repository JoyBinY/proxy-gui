#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <QObject>
#include "UserData.h"

class Configure : public QObject
{
////////////////////////////////////////////////////////////////////////////////////
    Q_OBJECT
    Q_PROPERTY(bool enable READ enable WRITE setEnable NOTIFY enableChanged)
    Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)

public:
    bool enable();
    void setEnable(bool enable);
    int index();
    void setIndex(int index);
    int port();
    void setPort(int port);
signals:
    void enableChanged();
    void indexChanged();
    void portChanged();
////////////////////////////////////////////////////////////////////////////////////

public:
    explicit Configure(ConfigureData *data, QObject *parent = nullptr);

public:
    Q_INVOKABLE void applyConfigure();

public Q_SLOTS:
    void onModelSaved(const QList<ProxyData>&);

private:
    void init_cfg_file();
    void write_cfg(ConfigureData *cfg);
    void read_cfg(ConfigureData *cfg);

private:
    const QString CFG_FILENAME;
    ConfigureData *m_data;

public slots:
};

#endif // CONFIGURE_H
