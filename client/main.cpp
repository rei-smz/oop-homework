#include "mainwindow.h"

#include <QApplication>

bool connectionStatus;
QTcpSocket clientSocket;
QHostAddress serverIp;
quint16 serverPort;
std::string username;
uint64_t userId;
double userBalance;
user_type_t userType;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    userType = guest;
    connectionStatus = false;
    MainWindow w;
    w.show();
    return a.exec();
}
