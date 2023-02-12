#pragma once

#include "TitledWidget.h"

class QCheckBox;
class QTextEdit;
class QLineEdit;
class QLabel;
class QComboBox;

class SendWidget : public TitledWidget
{
    Q_OBJECT

public:
    SendWidget(QWidget *parent);

private:
    void setupUi();

private slots:
    void sendData();
    void showModeDetail(int);
    void resetToTextMode();
    void openChooseFileDialog();

private:
    QComboBox *mode_;
    QCheckBox *intervalBox_;
    QLineEdit *intervalEdit_;
    QCheckBox *autoSend_;
    QTextEdit *sendEdit_;
};
