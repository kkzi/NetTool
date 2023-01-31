#include "LogWidget.h"
#include <QDateTime>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

LogWidget::LogWidget(QWidget *parent /*= nullptr*/)
    : QFrame(parent)
    , info_(new QLabel(this))
    , log_(new QTextEdit(this))
{
    setupUi();
    log_->setReadOnly(true);
    setMinimumHeight(120);
    setMaximumHeight(200);
}

void LogWidget::append(const QString &msg)
{
    auto now = QDateTime::currentDateTime();
    QString line = QString("[%1] %2").arg(now.toString(Qt::ISODate)).arg(msg);
    log_->append(line);
}

void LogWidget::setupUi()
{
    auto top = new QHBoxLayout;
    top->setContentsMargins(0, 0, 0, 0);
    {
        auto title = new QLabel("日志和统计信息");
        title->setObjectName("title");
        top->addWidget(title);
        top->addStretch();
        top->addWidget(info_);
        top->addSpacing(10);
        auto clearBtn = new QPushButton("清空");
        top->addWidget(clearBtn);
        connect(clearBtn, SIGNAL(clicked()), log_, SLOT(clear()));
        connect(clearBtn, SIGNAL(clicked()), info_, SLOT(clear()));
    }

    auto layout = new QVBoxLayout(this);
    layout->addLayout(top);
    layout->addWidget(log_, 1);
}
