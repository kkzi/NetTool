#include "LogWidget.h"
#include <QDateTime>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

LogWidget::LogWidget(QWidget *parent /*= nullptr*/)
    : TitledWidget(tr("Logs Information"), new QWidget, log_ = new QTextEdit, parent)
    , info_(new QLabel(this))
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
    auto top = new QHBoxLayout(corner_);
    top->setContentsMargins(0, 0, 0, 0);
    top->addWidget(info_);
    top->addSpacing(10);
    auto clearBtn = new QPushButton(tr("CLEAR"));
    top->addWidget(clearBtn);
    connect(clearBtn, &QPushButton::clicked, log_, &QTextEdit::clear);
    connect(clearBtn, &QPushButton::clicked, info_, &QLabel::clear);
}
