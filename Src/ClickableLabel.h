#pragma once

#include <QLabel>

class ClickableLabel : public QLabel
{
    Q_OBJECT

public:
    using QLabel::QLabel;

protected:
    void mouseReleaseEvent(QMouseEvent *ev);

signals:
    void mouseLeftButtonClicked();
    void mouseRightButtonClicked();
};
