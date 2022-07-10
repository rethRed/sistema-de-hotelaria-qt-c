#include "principal.h"
#include <QSettings>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Principal principal;


    principal.show();
    return a.exec();

}
