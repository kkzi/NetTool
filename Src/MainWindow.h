#pragma once

#include <QFrame>

class FileStorage;

class MainWindow : public QFrame
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private:
    void setupUi();

private:
    FileStorage *storage_;
};
