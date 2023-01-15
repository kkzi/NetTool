#pragma once

#include <QFrame>

class QTextEdit;
class QCheckBox;
class QLabel;
class QLineEdit;


class RecvWidget : public QFrame
{
    Q_OBJECT

public:
    RecvWidget(QWidget* parent);

private:
    void setupUi();

private:
    QTextEdit* recvEdit_;
    QCheckBox* hexMode_;
    QLabel* filePath_;
    QLineEdit* bufferLimit_;
};

