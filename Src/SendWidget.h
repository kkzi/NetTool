#pragma once

#include <QFrame>

class QCheckBox;
class QTextEdit;
class QLineEdit;
class QLabel;
class QComboBox;

class SendWidget : public QFrame
{
    Q_OBJECT

public:
    SendWidget(QWidget *parent);

private:
    void setupUi();

private slots:
    void sendData();
    void showModeDetail(int);

private:
    QComboBox *mode_;
    QLineEdit *bufferLimit_;
    QCheckBox *intervalBox_;
    QLineEdit *intervalEdit_;
    QCheckBox *autoSend_;
    QTextEdit *sendEdit_;
    QLabel *filePath_;
};
