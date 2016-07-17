#include <QtGlobal>
#include <QDebug>
#include <QApplication>
#include <cstdlib>
#include <QElapsedTimer>
#include <QDebug>
#include <QtNetwork>
#include <QTcpServer>
#include <QTcpSocket>
#include "client.h"

Client::Client(QObject* parent)
    : QObject(parent)
    , m_it(0)
{
  connect(&client, SIGNAL(connected()),
          this, SLOT(sendUsers()));
  timer.start();

  namesList << "Jackson";
  namesList << "Hebert";
  namesList << "John";
  namesList << "Claire";
  namesList << "Sarah";
  namesList << "John";
}

Client::~Client()
{
  client.close();
}

void
Client::readResponse()
{
    char buffer[1024] = {0};
    client.read(buffer, client.bytesAvailable());
    answer = buffer;
    client.close();
}

void
Client::sendUsers()
{
    if (m_it >= namesList.size()){
        //qDebug() << "No more users to send";
        this->endApp();
        return;
    }

    QString name = namesList.at(m_it);
    qint16 randomNum = -1;
    for (qint16 i = 0; i < 250; i++){
        randomNum = generateRandomNumber();
        if (randomNum != -1)
            break;
    }
    if (randomNum == -1){
        //qDebug() << "No more Ids available";
        this->endApp();
        return;
    }

    this->sendUsers(name, randomNum);
    this->connectToServer("127.0.0.1",8888);
    m_it += 1;

}

void
Client::sendUsers(QString name, qint16 randomNum)
{
    timer.restart();
    QString nId = QString::number(randomNum);
    if (nId.length() < 2)
        nId = "00" + nId;
    else if (nId.length() < 3)
        nId = "0" + nId;
    QString user = nId + "$" + name;
    client.write(user.toStdString().c_str(), int(user.length()+1));
    connect(&client, SIGNAL(readyRead()),
            this, SLOT(readResponse()));
    while(client.waitForReadyRead(3000));
    qInfo() << "User: " << name << " Time elapsed: " << timer.elapsed()
             << "ms. Server status:" << answer;
    answer = "";
    disconnect(&client, SIGNAL(readyRead()),
            this, SLOT(readResponse()));
    sleep(1);
    client.close();
}


void
Client::connectToServer(QString address, quint16 port)
{
    QHostAddress addr(address);
    if(client.state() == QTcpSocket::ConnectedState){
        //qDebug() << "Client already connected";
    }
    else {
        //qDebug() << "Connecting to server";
        client.connectToHost(addr, port,QIODevice::ReadWrite);
    }
}

qint16
Client::generateRandomNumber(){
    qint16 num;
    qsrand(qrand());
    int tryNum = qrand() % (250 + 1);
    if (tryNum < 1 || tryNum > 250) {
        return -1;
    }
    for(auto it : m_randomNumVec){
        if (tryNum == it){
            return -1;
        }
      }

    num = tryNum;
    m_randomNumVec.push_back(num);
    return num;

}

void
Client::endApp()
{
    disconnect(&client, SIGNAL(connected()),
            this, SLOT(sendUsers()));
    QCoreApplication::quit();
}
