#include "App.h"
#include "MainWindow.h"
#include <QWidget>

int main(int argc, char **argv)
{
    App a(argc, argv);

    MainWindow w;
    w.setWindowTitle("Net Assistat Tool v1.3 beta");
    w.show();
    return a.exec();
}
