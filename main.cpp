#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Firebird browser");
    a.setOrganizationName("RTVSystem");
    a.setOrganizationDomain("soft21.pl");
    MainWindow w;
    w.show();

    return a.exec();
}
