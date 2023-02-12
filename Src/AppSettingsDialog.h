#pragma once

#include <QDialog>

class QComboBox;

class AppSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    AppSettingsDialog(QWidget *parent = nullptr);

private slots:
    void updateSettings();

private:
    QComboBox *themeBox_;
    QComboBox *langBox_;
};
