#pragma once


#include <QFrame>

class SideBar;


class Form : public QFrame
{
    Q_OBJECT

public:
    Form();


private:
    void setupUi();

private:
    SideBar *side_;
};

