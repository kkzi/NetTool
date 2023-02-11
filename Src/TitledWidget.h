#pragma once

#include <QFrame>
#include <QIcon>

class QLabel;
class QLayout;

class TitledWidget : public QFrame
{
    Q_OBJECT

public:
    TitledWidget(const QString &title, QWidget *corner, QWidget *central, QWidget *parent = nullptr);
    virtual ~TitledWidget();

protected:
    QWidget *corner_{ nullptr };
    QWidget *central_{ nullptr };
};
