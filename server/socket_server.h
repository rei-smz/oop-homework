#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <nlohmann/json.hpp>

extern QTcpServer tcpserver;
extern quint16 port;
extern QList<QTcpSocket*> clientList;
extern QTimer timer;
extern bool isRunning;

void server_init();
void send_callback(const nlohmann::json msg, uint socket_id);

#endif // SOCKET_SERVER_H
