#include "App.h"
#include "MainWindow.h"
#include <QWidget>
#include <QtCore/QTextCodec>

int main(int argc, char **argv)
{
    App a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

    MainWindow w;
    w.setWindowTitle("Net Assistat Tool v1.1 beta");
    w.show();
    return a.exec();
}
