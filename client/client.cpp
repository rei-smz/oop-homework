#include "client.h"

bool connect2Server(const QString addr, uint16_t port) {
    if(!connectionStatus) {
        serverIp.setAddress(addr);
        serverPort = quint16(port);
        clientSocket.connectToHost(serverIp, serverPort);
        if(clientSocket.waitForConnected()) {
            connectionStatus = true;
            qDebug() << "连接成功";
            return connectionStatus;
        } else {
            qWarning() << "连接失败";
            return false;
        }
    }
    return false;
}

QByteArray wait4Msg() {
    if(clientSocket.bytesAvailable()) {
        QByteArray buf = clientSocket.readAll();
        return buf;
    }
    return nullptr;
}

void sendMsg(const nlohmann::json msg) {
    std::string msgString = msg.dump();
    qDebug() << "send:" <<QString::fromStdString(msgString);
    clientSocket.write(QByteArray::fromStdString(msgString));
}

void disconnect() {
    if(connectionStatus) {
        clientSocket.disconnectFromHost();
        qDebug() << "断开成功";
    }
}
