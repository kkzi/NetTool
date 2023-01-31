#include "FileStorage.h"
#include "NetworkTaskManager.h"
#include <QMessageBox>

FileStorage::~FileStorage()
{
    stop();
}

void FileStorage::setPath(const QString &path)
{
    if (path == file_.fileName())
    {
        return;
    }
    if (file_.isOpen())
    {
        stop();
        file_.setFileName(path);
        start();
    }
    else
    {
        file_.setFileName(path);
    }
}

bool FileStorage::start()
{
    return file_.open(QFile::WriteOnly);
}

void FileStorage::stop()
{
    if (file_.isOpen())
    {
        file_.close();
    }
}

QString FileStorage::path() const
{
    return file_.fileName();
}

void FileStorage::saveToFile(const QString &from, const QByteArray &data)
{
    /// todo: 区分不同客户端发送的数据
    /// todo fix this
    //Q_UNUSED(from);
    //Q_ASSERT(file_.isOpen());
    //file_.write(data);
}
