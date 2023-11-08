#include <QApplication>
#include "mainwindow.h"
#include "mywidget.h"

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);


    MainWindow window;
    window.show();

    MyWidget myWidget;
    myWidget.show();


    return app.exec();
}
