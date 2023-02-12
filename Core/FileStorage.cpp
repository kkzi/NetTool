#include "FileStorage.h"
#include "NetworkTaskManager.h"
#include <QMessageBox>

FileStorage::~FileStorage()
{
    stop();
}

void FileStorage::setPath(const QString &path)
{
    if (filepath_ == path)
    {
        return;
    }

    if (file_.isOpen())
    {
        file_.close();
    }

    filepath_ = path;
}

bool FileStorage::start()
{
    isRunning_ = true;
    return true;
}

void FileStorage::stop()
{
    isRunning_ = false;
    if (file_.isOpen())
    {
        file_.close();
    }
}

void FileStorage::saveToFile(const QString &from, const QByteArray &data)
{
    /// todo: 区分不同客户端发送的数据
    Q_UNUSED(from);

    if (filepath_.isEmpty())
    {
        return;
    }

    ensureOpenFile();
    Q_ASSERT(file_.isOpen());
    file_.write(data);
}

void FileStorage::ensureOpenFile()
{
    if (!file_.isOpen())
    {
        file_.setFileName(filepath_);
        file_.open(QFile::WriteOnly | QFile::Truncate);
    }
}
