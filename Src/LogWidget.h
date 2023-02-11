#pragma once

#include "TitledWidget.h"

class QTextEdit;
class QLabel;

class LogWidget : public TitledWidget
{
    Q_OBJECT

public:
    LogWidget(QWidget *parent = nullptr);

public slots:
    void append(const QString &msg);

private:
    void setupUi();

private:
    QLabel *info_;
    QTextEdit *log_;
};
