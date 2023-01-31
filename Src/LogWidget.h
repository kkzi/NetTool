#pragma once

#include <QFrame>

class QTextEdit;
class QLabel;

class LogWidget : public QFrame
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
