#pragma once

#include "TitledWidget.h"
#include <QByteArray>
#include <QFrame>

class QTimer;
class QTextEdit;
class QCheckBox;
class QLabel;
class QLineEdit;
class QComboBox;
class ClickableLabel;

class RecvWidget : public TitledWidget
{
    Q_OBJECT

public:
    RecvWidget(QWidget *parent);
    ~RecvWidget();

public:
    bool isFileMode() const;
    QString storageFilePath() const;

private:
    void setupUi();

private slots:
    void updateBufferDisplay();

public slots:
    void append(const QString &from, const QByteArray &data);

private slots:
    void setSaveFileEnabled(bool);
    void openStoreFilePath();
    void openStoreDir();
    void cleanBuffer();

private:
    QCheckBox *saveFile_;
    ClickableLabel *filePath_;
    QComboBox *mode_;
    QTextEdit *recvEdit_;
    QByteArray buffer_;
    QTimer *timer_;
};
