#include "socket_server.h"

void server_init() {
    port = 6666;
    isRunning = false;
}

void send_callback(const nlohmann::json msg, uint socket_id) {
    std::string msg_s = msg.dump();
    qDebug() << "send:" << QString::fromStdString(msg_s);
    clientList.at(socket_id)->write(QByteArray::fromStdString(msg_s));
}
