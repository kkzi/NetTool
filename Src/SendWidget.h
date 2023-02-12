#pragma once

#include "TitledWidget.h"

class QCheckBox;
class QTextEdit;
class QLineEdit;
class QLabel;
class QComboBox;
class QTimer;

class SendWidget : public TitledWidget
{
    Q_OBJECT

public:
    SendWidget(QWidget *parent);

private:
    void setupUi();
    QByteArray prepareData() const;

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
    QTextEdit *sendEdit_;
    QTimer *timer_;
};
