#include "ClickableLabel.h"
#include <QMouseEvent>

void ClickableLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    switch (ev->button())
    {
    case Qt::LeftButton:
        emit mouseLeftButtonClicked();
        break;
    case Qt::RightButton:
        emit mouseRightButtonClicked();
        break;
    default:
        break;
    }
    return QLabel::mouseReleaseEvent(ev);
}

