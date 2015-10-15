#include <QApplication>

#include "xmudialog.h"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    XmuDialog w;
    w.show();
    return app.exec();
}
