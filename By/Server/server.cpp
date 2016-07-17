#include <QApplication>
#include <QDebug>
#include <QTextStream>
#include <QFile>
#include <QElapsedTimer>
#include "server.h"
#include <QTimer>

#include <iostream>

Server::Server(QObject* parent): QObject(parent)
{
  timer = new QTimer(this);
  connect(&server, SIGNAL(newConnection()),
          this, SLOT(acceptConnection()));
  connect(timer, SIGNAL(timeout()),
          this, SLOT(killServer()));
  timer->start(60000);
  server.listen(QHostAddress::Any, 8888);

}

Server::~Server()
{
  server.close();
}

void
Server::acceptConnection()
{
    client = server.nextPendingConnection();
    connect(client, SIGNAL(readyRead()),
            this, SLOT(startRead()));
}

void
Server::startRead()
{
    timer->start(60000);
    char buffer[1024] = {0};
    client->read(buffer, client->bytesAvailable());
    QString user(buffer);
    qintptr socketDesc = server.socketDescriptor();
    this->addUser(QString::number(socketDesc)+"&"+user);
}

void
Server::sendResponse(std::string response){
    client->write(response.c_str(),response.length()+1);
    //client = server.nextPendingConnection();
    client->close();
}


bool
Server::addUser(QString user_id, int n)
{
    foreach (QString name, m_userList){
        if (name.contains(user_id, Qt::CaseInsensitive)){
            //qWarning() << "Client tried to insert existing user";
            return false;
        }
    }
    return true;
}

void
Server::addUser(QString user)
{
    QStringList list = user.split("&");
    QString user_id = list.at(1); // id$user
    QString client_name = list.at(0); //client socket number
    if(!this->addUser(user_id, 1)){
        this->sendResponse("User and id already exists");
    }
    else {
        m_userList.append(user);
        this->sendResponse("User added");
        //qDebug() << "User aded";
    }
}

void
Server::killServer()
{
    qInfo() << "Closing server";
    if(!this->saveListTofile()){
        qFatal("Failed to save data");
    }
    QApplication::quit();
}

bool
Server::saveListTofile()
{
    QFile file(m_filename);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        foreach (QString id, m_userList){
            id.replace("&","$");
            QStringList whole = id.split("$");
            QString client_id = whole.at(0);
            QString ID = whole.at(1);
            QString user_name = whole.at(2);
            stream << "Username:\t" <<user_name.toStdString().c_str();
            stream << "\tClient ID:\t" << client_id.toStdString().c_str() << endl;
        }
        file.close();
        return true;
    }
    else{
        return false;
    }
    return true;
}
