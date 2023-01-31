#include "NetworkTaskManager.h"
#include "FileStorage.h"
#include "NetworkTask.h"

NetworkTaskManager::NetworkTaskManager(QObject *parent /*= nullptr*/)
    : QObject(parent)
    , io_(1)
    , guard_(boost::asio::make_work_guard(io_))
    , storage_(new FileStorage(this))
{
    bool ok = connect(this, SIGNAL(dataReceived(QString, QByteArray)), storage_, SLOT(saveToFile(QString, QByteArray)),
        Qt::QueuedConnection);
    ok = false;
}

NetworkTaskManager *NetworkTaskManager::instance()
{
    static NetworkTaskManager ins;
    return &ins;
}

NetworkTask *NetworkTaskManager::create(const QString &name)
{
    return creators_.contains(name) ? creators_.value(name)() : nullptr;
}

NetworkTask *NetworkTaskManager::start(const QString &name, const NetworkConfig &conf)
{
    Q_ASSERT(current_ == nullptr);

    /// todo 处理文件保存和各模式间的关系
    // if (!storage_->start())
    //{
    //    emit logMessage(QString("保存文件失败 %1").arg(storage_->path()));
    //    return nullptr;
    //}

    auto task = create(name);
    Q_CHECK_PTR(task);
    connect(task, SIGNAL(logMessage(QString)), this, SIGNAL(logMessage(QString)), Qt::QueuedConnection);
    connect(task, SIGNAL(dataReceived(QString, QByteArray)), this, SIGNAL(dataReceived(QString, QByteArray)),
        Qt::QueuedConnection);

    task->setConfig(conf);
    io_.restart();
    auto &&[ok, err] = task->start(io_);
    if (!ok)
    {
        throw std::runtime_error(err.toStdString());
    }
    current_ = task;

    thread_ = std::thread([this] {
        io_.run();
    });

    return current_;
}

NetworkTask *NetworkTaskManager::current()
{
    return current_;
}

void NetworkTaskManager::stopCurrent()
{
    storage_->stop();
    if (!io_.stopped())
    {
        io_.reset();
        io_.stop();
    }
    if (thread_.joinable())
    {
        thread_.join();
    }
    if (current_)
    {
        delete current_;
        current_ = nullptr;
    }
}

void NetworkTaskManager::setStoragePath(const QString &path)
{
    storage_->setPath(path);
}
