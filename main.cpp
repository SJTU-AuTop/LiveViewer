#include "liveviewer.h"

#include <QApplication>
#include <QChartView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LiveViewer w;
    w.show();

// FOR DEBUG
//    w.updateImage("img0");
//    w.updateImage("img1");
//    w.updateParam("p0", 0.1, 2.1, 0.21, 1);
//    w.updateParam("p1", -1.2, 0.12, 0.01, -0.5);
//    w.updateOption("opt0", true);
//    w.updateButton("b0");


    return a.exec();
}
