﻿#include "MainWindow.h"
#include "ConfigWidget.h"
#include "LogWidget.h"
#include "NetworkTaskManager.h"
#include "RecvWidget.h"
#include "SendWidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QStatusBar>
#include <QTextEdit>

MainWindow::MainWindow()
    : QFrame()
{
    setupUi();
}

MainWindow::~MainWindow()
{
    NetworkTaskManager::instance()->stopCurrent();
}

void MainWindow::setupUi()
{
    RecvWidget *recw;
    auto logw = new LogWidget(this);
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 0, 10, 10);
    layout->setSpacing(6);
    layout->addWidget(new ConfigWidget(this));
    layout->addWidget(recw = new RecvWidget(this), 4);
    layout->addWidget(new SendWidget(this), 3);
    layout->addWidget(logw, 2);
    // layout->addWidget(new QStatusBar);
    resize(960, 720);

    auto ntm = NetworkTaskManager::instance();
    connect(ntm, &NetworkTaskManager::logMessage, logw, &LogWidget::append, Qt::QueuedConnection);
    connect(ntm, &NetworkTaskManager::dataReceived, recw, &RecvWidget::append, Qt::QueuedConnection);
}