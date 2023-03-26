#include "NetworkTaskManager.h"
#include "FileStorage.h"
#include "NetworkTask.h"

NetworkTaskManager::NetworkTaskManager(QObject *parent /*= nullptr*/)
    : QObject(parent)
    , storage_(new FileStorage(this))
{
    connect(this, &NetworkTaskManager::dataReceived, storage_, &FileStorage::saveToFile, Qt::QueuedConnection);
}

NetworkTaskManager::~NetworkTaskManager()
{
}

NetworkTaskManager *NetworkTaskManager::instance()
{
    static NetworkTaskManager ins;
    return &ins;
}

NetworkTask *NetworkTaskManager::create(const QString &name, const NetworkConfig &conf)
{
    auto task = creators_.contains(name) ? creators_.value(name)() : nullptr;
    if (task != nullptr)
    {
        connect(task, &NetworkTask::logMessage, this, &NetworkTaskManager::logMessage, Qt::QueuedConnection);
        connect(task, &NetworkTask::dataReceived, this, &NetworkTaskManager::dataReceived, Qt::QueuedConnection);
    }
    task->setConfig(conf);

    return task;
}

void NetworkTaskManager::start(NetworkTask *task)
{
    Q_CHECK_PTR(task);

    task->start();
    current_ = task;
}

NetworkTask *NetworkTaskManager::current()
{
    return current_;
}

void NetworkTaskManager::stopCurrent()
{
    storage_->stop();
    if (current_)
    {
        current_->stop();
        delete current_;
        current_ = nullptr;
    }
}

void NetworkTaskManager::setStoragePath(const QString &path)
{
    storage_->setPath(path);
}
