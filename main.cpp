#include "colorpicker.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ColorPicker w;
    w.show();

    return a.exec();
}
