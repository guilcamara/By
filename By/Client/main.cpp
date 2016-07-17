#include <QApplication>
#include "client.h"

int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    Client client;
    client.connectToServer("127.0.0.1", 8888);
    return app.exec();
}
