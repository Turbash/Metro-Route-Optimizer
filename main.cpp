#include <QApplication>
#include "MetroPlannerWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MetroPlannerWindow window;
    window.show();
    return app.exec();
}
