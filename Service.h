#ifndef SERVICE_H
#define SERVICE_H

#include <QObject>
#include <QString>
#include <QProcess>

class ConfigureData;
class Service : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString tooltip READ tooltip WRITE setTooltip NOTIFY tooltipChanged)

public:
    explicit Service(ConfigureData *data, QObject *parent = nullptr);
    Q_INVOKABLE void stop();
    Q_INVOKABLE bool startUp();
    Q_INVOKABLE void setStartUp(bool should);
    void startWork();

public:
    QString tooltip();
    void setTooltip(const QString &msg);

public Q_SLOTS:
    void onConnectChanged();

signals:
    void tooltipChanged();

private:
    QString get_command();
    void enable_ie_proxy(bool enable);

private slots:
    void on_state_changed(QProcess::ProcessState newState);
    void on_error_occurred(QProcess::ProcessError error);
    void on_finished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    static QString REG_RUN;
    static QString REG_IE_SETTING;
    ConfigureData *m_data;
    QProcess m_process;
    QString m_msg;
};

#endif // SERVICE_H
