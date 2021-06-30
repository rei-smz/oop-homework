#ifndef CLIENT_H
#define CLIENT_H

#define CMD_TEST 0
#define CMD_LOGIN 1
#define CMD_LOGOUT 13
#define CMD_REG 2
#define CMD_CHG_PASS 3
#define CMD_ADD_GOODS 4
#define CMD_DEL_GOODS 5
#define CMD_CHG_REMAIN 6
#define CMD_CHG_OPRICE 7
#define CMD_CHG_DISCOUNT 8
#define CMD_ADD_CART 9
#define CMD_DEL_CART 10
#define CMD_CHG_CART 15
#define CMD_PURCHASE 11
#define CMD_RE_CART 12
#define CMD_RE_GOODS 14
#define CMD_RECHARGE 16

#define CB_TEST_OK 0
#define CB_RE_USER 1
#define CB_REG 2
#define CB_CHG_PASS 3
#define CB_ADD_GOODS 4
#define CB_DEL_GOODS 5
#define CB_CHG_REMAIN 6
#define CB_CHG_OPRICE 7
#define CB_CHG_DISCOUNT 8
#define CB_ADD_CART 9
#define CB_DEL_CART 10
#define CB_CHG_CART 14
#define CB_PURCHASE 11
#define CB_RE_CART 12
#define CB_RE_GOODS 13
#define CB_RECHARGE 15

#include <QTcpSocket>
#include <QHostAddress>
#include <nlohmann/json.hpp>

enum user_type_t {merchant=0,customer=1,guest=2}; //guest为未登录
enum goods_type_t {food=0,cloths=1,book=2,electronic=3};

extern bool connectionStatus;
extern QTcpSocket clientSocket;
extern QHostAddress serverIp;
extern quint16 serverPort;
extern std::string username;
extern uint64_t userId;
extern double userBalance;
extern user_type_t userType;


bool connect2Server(const QString addr, uint16_t port);
void disconnect();
QByteArray wait4Msg();
void sendMsg(const nlohmann::json msg);
#endif // CLIENT_H
