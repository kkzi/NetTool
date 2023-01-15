#pragma once

#include <QWidget>

class QComboBox;
class QLineEdit;
class QStackedWidget;
class QPushButton;

class SideBar : public QWidget
{
    Q_OBJECT

public:
    SideBar(QWidget* parent);

public:
    void registerProto(QString, QWidget*);

private:
    void setupUi();


private:
    QComboBox* proto_;
    QComboBox* ip_;
    QLineEdit* port_;
    QStackedWidget* protoDetail_;
    QPushButton* start_;
};

