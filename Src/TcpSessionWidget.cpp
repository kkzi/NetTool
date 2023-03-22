#include "TcpSessionWidget.h"
#include "NetworkTaskManager.h"
#include "TcpServerTask.h"
#include <QListView>
#include <QStandardItemModel>
#include <QTimerEvent>
#include <QVBoxLayout>

void TcpSessionModel::setup(const QStringList &arr)
{
    beginResetModel();
    data_ = arr;
    endResetModel();
}

int TcpSessionModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    return data_.size();
}

QVariant TcpSessionModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
    if (!index.isValid())
    {
        return {};
    }
    if (role == Qt::DisplayRole)
    {
        return data_.value(index.row());
    }
    return {};
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

TcpSessionWidget::TcpSessionWidget(QWidget *parent /*= nullptr*/)
    : QWidget(parent)
    , list_(new QListView((this)))
    , model_(new TcpSessionModel(this))
{
    list_->setModel(model_);
    list_->setFixedHeight(80);
    setupUi();
}

void TcpSessionWidget::onTaskStarted()
{
    auto task = NetworkTaskManager::instance()->current();
    Q_CHECK_PTR(task);
    bool ok =
        connect(task, SIGNAL(sessionChanged(QStringList)), model_, SLOT(setup(QStringList)), Qt::QueuedConnection);
    Q_ASSERT(ok);
}

void TcpSessionWidget::onTaskStopped()
{
    model_->setup({});
}

void TcpSessionWidget::setupUi()
{
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(list_, 1);
}

void TcpSessionWidget::updateModel()
{
    // auto task = qobject_cast<TcpServerTask *>(NetworkTaskManager::current());
    // if (task == nullptr)
    //{
    //    return;
    //}
    QList<QStandardItem *> rows;
    // for (auto &&c : task->clients())
    //{
    //    rows << new QStandardItem(c);
    //}
    // model_->clear();
    // model_->appendRow(rows);
}
