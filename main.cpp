#include "mainwindow.h"
#include <QApplication>

#include "stdafx.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.setWindowTitle("Data Simulation");
    CommonHelper::setStyle(":/black_theme.qss");
    w.show();
    return a.exec();
}
