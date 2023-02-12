#pragma once

#include "NetworkConfig.h"
#include "NetworkTaskManager.h"
#include "TitledWidget.h"
#include <QComboBox>
#include <QMap>
#include <QStackedWidget>
#include <boost/asio/io_context.hpp>
#include <functional>
#include <thread>

class QLineEdit;
class QPushButton;
class NetworkTask;

class ConfigWidget : public TitledWidget
{
    Q_OBJECT

public:
    ConfigWidget(QWidget *parent);

public:
    template <class T, class W>
    void registerProto(QString name)
    {
        NetworkTaskManager::instance()->registar<T>(name);
        proto_->addItem(name);
        protoDetail_->addWidget(new W(name, this));
    }

private:
    void setupUi();
    void startNewNetworkTask();
    void stopCurrentNetworkTask();
    NetworkConfig gatherConfig(QWidget *detail) const;

private slots:
    void ctrlNetworkTask(bool);
    void updateCtrlButtons(int);
    void showDetailWidget(int);
    void showSettingsDialog();

private:
    QComboBox *proto_;
    QComboBox *localIp_;
    QLineEdit *localPort_;
    QStackedWidget *protoDetail_;
    QWidget *form_;
    QPushButton *start_;
};
