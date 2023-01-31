#include "Form.h"
#include <QApplication>
#include <QWidget>
#include <QtCore/QTextCodec>

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

    Form w;
    w.setStyleSheet(R"(
*{font:normal 12px "Microsoft Yahei";}
QFrame{background-color:#fff;outline:none;}
QLabel#title{font-size:14px;font-weight:bold;}
RecvWidget QTextEdit, LogWidget QTextEdit{font:normal 13px "Consolas, monospace";}
TcpSessionWidget QListView {border:0; padding:10px; }
        )");
    w.show();
    return a.exec();
}
