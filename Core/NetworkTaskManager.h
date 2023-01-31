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

public:
    static NetworkTaskManager *instance();

    template <class T>
    void registar(const QString &name)
    {
        creators_[name] = []() {
            return new T;
        };
    }

    NetworkTask *create(const QString &name);
    NetworkTask *current();
    NetworkTask *start(const QString &name, const NetworkConfig &);
    void stopCurrent();
    void setStoragePath(const QString &path);

signals:
    void logMessage(const QString &);
    void dataReceived(const QString &, const QByteArray &);

private:
    boost::asio::io_context io_;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> guard_;
    std::thread thread_;
    NetworkTask *current_;
    QMap<QString, std::function<NetworkTask *()>> creators_;
    FileStorage *storage_{ nullptr };
};
