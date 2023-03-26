#pragma once

#include "TitledWidget.h"

class QCheckBox;
class QTextEdit;
class QLineEdit;
class QLabel;
class QComboBox;
class QPushButton;
class QTimer;

class SendWidget : public TitledWidget
{
    Q_OBJECT

public:
    SendWidget(QWidget *parent);

private:
    void setupUi();
    QByteArray prepareData();
    bool isFileMode() const;

private slots:
    void sendData();
    void showModeDetail(int);
    void resetToTextMode();
    void openChooseFileDialog();
    void ctrlSendTimer(bool);

private:
    QComboBox *mode_;
    QCheckBox *intervalBox_;
    QLineEdit *intervalEdit_;
    QLineEdit *frameLimitEdit_;
    QCheckBox *autoSend_;
    QPushButton *sendButton_{ nullptr };
    QTextEdit *sendEdit_;
    QTimer *timer_;
    size_t offset_{ 0 };
};
