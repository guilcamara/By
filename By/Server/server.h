#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QtNetwork>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();
    bool addUser(QString userid,int n);
    void addUser(QString user);
    bool saveListTofile();
    void sendResponse(std::string response);

private:
    QStringList m_userList;
    const QString m_filename = "/tmp/clientUsers.txt";
    QTcpServer server;
    QTcpSocket* client;
    QTimer *timer;

public slots:
  void acceptConnection();
  void startRead();
  void killServer();



};

#endif // SERVER_H
