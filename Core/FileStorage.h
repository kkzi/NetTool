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
    QString path() const;

public slots:
    void saveToFile(const QString &from, const QByteArray &data);

private:
    QFile file_;
};
