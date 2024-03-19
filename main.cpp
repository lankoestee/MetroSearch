//main.cpp

//Line.h,Line.cpp,Station.h,Station.cpp,MetroSearch.h,MetroSearch.cpp,getInfo.py

#include "MetroSearch.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MetroSearch w;
    w.show();
    return a.exec();
}
