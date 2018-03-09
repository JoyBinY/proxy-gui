#include "Logger.h"
#include <QDateTime>

void Logger::initLogger() {
    m_file.setFileName(LOG_NAME);
    if (m_file.exists()) {
        m_file.remove();
    }
    m_file.open(QIODevice::WriteOnly | QIODevice::Append);
    m_stream.setDevice(&m_file);
}

bool Logger::releaseLogger() {
    bool ret = m_file.flush();
    m_file.close();
    return ret;
}

void Logger::addLogRecord(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QString text;
    switch (type) {
    case QtDebugMsg:
        text = "DEBUG";
        break;
    case QtInfoMsg:
        text = "INFO";
        break;
    case QtWarningMsg:
        text = "WARNING";
        break;
    case QtCriticalMsg:
        text = "CRITICAL";
        break;
    case QtFatalMsg:
        text = "FATAL";
        break;
    }

    const QString &stamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
#ifdef QT_DEBUG
    const QString &info = QString("%1@%2@%3 %4").arg(context.file).
            arg(context.function).arg(context.line).arg(context.category);
    const QString &log_msg = QString("[%1(%2)]:%3(%4)").arg(text).arg(stamp).arg(msg).arg(info);
#else
    const QString &log_msg = QString("[%1(%2)]:%3").arg(text).arg(stamp).arg(msg);
#endif

    QWriteLocker lock(&m_mutex);
    m_stream << log_msg << "\r\n";
    m_stream.flush();
}

QString Logger::LOG_NAME = "gui-log.txt";
QFile Logger::m_file;
QTextStream Logger::m_stream;
QReadWriteLock Logger::m_mutex;
