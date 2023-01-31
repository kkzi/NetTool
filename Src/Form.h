#pragma once

#include <QFrame>

class SideBar;
class RecvWidget;
class SendWidget;
class FileStorage;

class Form : public QFrame
{
    Q_OBJECT

public:
    Form();
    ~Form();

private:
    void setupUi();

private:
    SideBar *side_;
    RecvWidget *recv_;
    SendWidget *send_;
    QWidget *logWidget_;

    FileStorage *storage_;
};
