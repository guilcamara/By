#include <QApplication>
#include <QElapsedTimer>
#include <QDebug>
#include "server.h"

int main(int argc, char **argv)
{
 QApplication app (argc, argv);
 Server server;

 return app.exec();
}
