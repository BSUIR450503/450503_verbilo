#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow MrChord;
    MrChord.show();

    return a.exec();
}
