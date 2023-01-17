#pragma once

#include "NetwokTaskFactory.h"
#include "NetworkConfig.h"
#include <QComboBox>
#include <QMap>
#include <QStackedWidget>
#include <QWidget>
#include <functional>

class QLineEdit;
class QPushButton;
class NetworkTask;

class SideBar : public QWidget
{
    Q_OBJECT

public:
    SideBar(QWidget *parent);

public:
    template <class T, class W>
    void registerProto(QString name)
    {
        NetworkTaskFactory::registar<T>(name);
        proto_->addItem(name);
        protoDetail_->addWidget(new W(this));
    }

private:
    void setupUi();
    void startNewNetworkTask();
    void stopCurrentNetworkTask();
    NetworkConfig gatherConfig() const;

private slots:
    void ctrlNetworkTask(bool);

private:
    QWidget *form_;
    QComboBox *proto_;
    QComboBox *localIp_;
    QLineEdit *localPort_;
    QStackedWidget *protoDetail_;
    QPushButton *start_;

    NetworkTask *current_{ nullptr };
};
