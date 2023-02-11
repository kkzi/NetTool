#pragma once

#include "NetworkTask.h"
#include <QMap>
#include <QString>
#include <boost/asio/io_context.hpp>
#include <functional>
#include <memory>
#include <thread>

struct NetworkConfig;
class FileStorage;

class NetworkTaskManager : public QObject
{
    Q_OBJECT

private:
    NetworkTaskManager(QObject *parent = nullptr);
    ~NetworkTaskManager();

public:
    static NetworkTaskManager *instance();

    template <class T>
    void registar(const QString &name)
    {
        creators_[name] = []() {
            return new T;
        };
    }

    NetworkTask *create(const QString &name, const NetworkConfig &);
    NetworkTask *current();
    void start(NetworkTask *);
    void stopCurrent();
    void setStoragePath(const QString &path);

signals:
    void logMessage(const QString &);
    void dataReceived(const QString &, const QByteArray &);

private:
    NetworkTask *current_;
    QMap<QString, std::function<NetworkTask *()>> creators_;
    FileStorage *storage_{ nullptr };
};
