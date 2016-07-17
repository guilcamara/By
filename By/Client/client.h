#ifndef CLIENT_H
#define CLIENT_H

#include <QtNetwork>
#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QElapsedTimer>

class Client : public QObject
{
    Q_OBJECT
public:
    Client(QObject* parent = 0);
      ~Client();

    void connectToServer(QString address, quint16 port);
    qint16 generateRandomNumber();
    void endApp();
    void sendUsers(QString name, qint16 randomNum);

private:
    std::vector<int> m_randomNumVec;
    QTcpSocket client;
    QStringList namesList;
    QElapsedTimer timer;
    QString answer;
    qint16 m_it;

public slots:
  void sendUsers();
  void readResponse();

};

#endif // CLIENT_H
