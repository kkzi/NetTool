#include <QApplication>
#include <QWidget>
#include <QtCore/QTextCodec>
#include "Form.h"


int main(int argc, char** argv)
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

    Form w;
    w.setStyleSheet(R"(
*{font:normal 12px "Microsoft Yahei";}
QFrame{background-color:#fff;outline:none;}
QLabel#title{font-size:16px;font-weight:bold;}
        )");
    w.show();
    return a.exec();
}
