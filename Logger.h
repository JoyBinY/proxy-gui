#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QString>
#include <QTextStream>
#include <QWriteLocker>
#include <QMessageLogContext>

class Logger
{
public:
    static void initLogger();
    static bool releaseLogger();
    static void addLogRecord(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    static QString LOG_NAME;
    static QFile m_file;
    static QTextStream m_stream;
    static QReadWriteLock m_mutex;
};

#endif // LOGGER_H
