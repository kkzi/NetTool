#pragma once

#include <QFile>
#include <QObject>
#include <QString>

class FileStorage : public QObject
{
    Q_OBJECT

public:
    using QObject::QObject;
    ~FileStorage();

public:
    void setPath(const QString &path);
    bool start();
    void stop();

public slots:
    void saveToFile(const QString &from, const QByteArray &data);

private:
    void ensureOpenFile();

private:
    bool isRunning_{ false };
    QString filepath_;
    QFile file_;
};
