#pragma once

#include <QAbstractListModel>
#include <QStringList>
#include <QWidget>

class QListView;
class TcpServerTask;

class TcpSessionModel : public QAbstractListModel
{
    Q_OBJECT
public:
    using QAbstractListModel::QAbstractListModel;

public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

public slots:
    void setup(const QStringList &data);

private:
    QStringList data_;
};

class TcpSessionWidget : public QWidget
{
    Q_OBJECT
public:
    TcpSessionWidget(QWidget *parent = nullptr);

public slots:
    void onTaskStarted();
    void onTaskStopped();

private:
    void setupUi();
    void updateModel();

private:
    QListView *list_;
    TcpSessionModel *model_;
};
