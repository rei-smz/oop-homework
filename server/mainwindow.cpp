#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QElapsedTimer>
#include <QMessageBox>
#include "socket_server.h"
#include "management.h"
#include "file_io.h"
#include "db.h"

QTcpServer tcpserver;
quint16 port;
QList<QTcpSocket*> clientList;
QTimer timer;
bool isRunning;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server_init();
    connect(&tcpserver,SIGNAL(newConnection()),this,SLOT(onNewConnection()));
    connect(&timer,SIGNAL(timeout()),this,SLOT(checkClient()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::checkClient() {
    for (int i=0; i < clientList.size(); i++) {
        if(clientList.at(i)->state() == QAbstractSocket::UnconnectedState) {
            clientList.removeAt(i);
            i--;
        }
    }
    timer.start(3000);
}

void MainWindow::onNewConnection() {
    QTcpSocket *client_socket = tcpserver.nextPendingConnection();
    clientList.append(client_socket);
    connect(client_socket,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
}

void MainWindow::onReadyRead() {
    for(int i=0;i<clientList.size();i++) {
        if(clientList.at(i)->bytesAvailable()) {
            QByteArray buf = clientList.at(i)->readAll();
            std::string msg_s = buf.toStdString();
            get_command(msg_s, i);
        }
    }
}

void MainWindow::on_pushButton_clicked() //启动按钮
{
    if(!isRunning) {
        if(!tcpserver.listen(QHostAddress::Any,port)) {
            qDebug() << "启动失败";
        } else {
            qDebug() << "启动成功";
            ui->lineEdit->setEnabled(false);
            ui->pushButton_2->setEnabled(false);
            ui->pushButton->setText("停止");
            isRunning = true;
            timer.start(3000);
        }
    } else {
        tcpserver.close();
        qDebug() << "服务器已关闭";
        timer.stop();
        ui->lineEdit->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton->setText("启动");
        clientList.clear();
        isRunning = false;
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    bool success;
    quint16 temp = ui->lineEdit->text().toUShort(&success);
    if(!success) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","输入的端口号不合法",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    } else {
        port = temp;
    }
}

void MainWindow::get_command(const std::string msg, uint socket_id) {
    qDebug() << "receive:" <<QString::fromStdString(msg);
    using namespace nlohmann;
    json command = json::parse(msg);
    json callback;
    /***********命令格式设计**********
     * 所有命令均包含一个键值type，表示命令类型
     * 命令类型定义 0测试 1登录 2注册 3修改密码 4添加商品 5删除商品 6修改余量 7修改原价 8修改折扣 9添加购物车 10删除购物车 11购买商品 12刷新购物车 13退出登录
     */
    int cmd_type = command["type"];
    switch (cmd_type) {
    case CMD_TEST:
        callback["type"] = CB_TEST_OK;
        callback["msg"] = "ok";
        send_callback(callback, socket_id);
        break;
    case CMD_LOGIN:  {
        callback["type"] = CB_RE_USER;
        std::string t_name,t_pass;
        t_name = command["username"];
        t_pass = command["password"];
        auto u=user_list[t_name];
        if(u == nullptr) {
            callback["error"] = "cannot find this user";
        } else {
            if(t_pass == u->getUserPass()) {
                callback["error"] = nullptr;
                callback["uid"] = u->getUserId();
                callback["username"] = command["username"];
                callback["balance"] = u->getBalance();
                if(u->getUserType() == merchant) {
                    callback["utype"] = 0;
                } else {
                    callback["utype"] = 1;
                }
                user_status[u->getUserId()] = true;
            } else {
                callback["error"] = "wrong password"; 
            }
        }
        send_callback(callback, socket_id);
        break;
    }
    case CMD_LOGOUT: {
        uint64_t uid = command["uid"];
        user_status[uid] = false;
        callback["type"] = CB_RE_USER;
        callback["error"] = nullptr;
        callback["uid"] = -1; //特殊处理
        send_callback(callback, socket_id);
        break;
    }
    case CMD_REG: {
        std::string t_name,t_pass,type_s;
        user_type_t type;
        t_name = command["username"];
        t_pass = command["password"];
        type_s = command["userType"];
        auto u=user_list[t_name];
        callback["type"] = CB_REG;
        if(u!=nullptr) {
            callback["error"] = "user existed";
        } else {
            user_count++;
            uint64_t uid = user_count-1;
            if(type_s=="商家") {
                type=merchant;
            } else {
                type=customer;
            }
            User* nu;
            switch (type) {
                case merchant:
                    nu=new Merchant(uid,t_name,t_pass,0);
                    break;
                case customer:
                    nu=new Customer(uid,t_name,t_pass,0);
                    break;
            }
            user_list[t_name]=nu;
            id_to_user[uid]=nu;
            user_status.push_back(true); //注册后自动登录
            changeUserToJson(nu);
            writeUserConfig();
            callback["error"] = nullptr;
            callback["uid"] = uid;
            callback["username"] = command["username"];
            callback["userType"] = command["userType"];
        }
        send_callback(callback, socket_id);
        break;
    }
    case CMD_CHG_PASS: {
        uint64_t uid = command["uid"];
        User *u = id_to_user[uid];
        std::string old_pass,new_pass;
        old_pass = command["oldPass"];
        new_pass = command["newPass"];
        callback["type"] = CB_CHG_PASS;
        if(old_pass != u->getUserPass()) {
            callback["error"] = "wrong password";
        } else {
            if(old_pass == new_pass) {
                callback["error"] = "same password";
            } else {
                u->changePassword(old_pass, new_pass);
                std::string type_s;
                if(u->getUserType() == merchant) {
                    type_s = "Merchant";
                } else {
                    type_s = "Customer";
                }
                for(auto& u_obj:user_json_list[type_s]) {
                    if(u_obj["uid"]==u->getUserId()) {
                        u_obj["password"]=u->getUserPass();
                        break;
                    }
                }
                writeUserConfig();
                callback["error"] = nullptr;
            }
        }
        send_callback(callback, socket_id);
        break;
    }
    case CMD_ADD_GOODS: {
        //用户类型在客户端判断
        std::string ng_name,ng_desc,ng_type_s;
        goods_type_t ng_type;
        double ng_price;
        uint64_t ng_remain;
        ng_name = command["goodsName"];
        uint64_t uid = command["uid"];
        User *u = id_to_user[uid];
        callback["type"] = CB_ADD_GOODS;
        if(goods_list[std::pair(ng_name, uid)] != nullptr) {
            callback["error"] = "same goods";
        } else {
            //价格和余量的输入合法性在客户端判断
            ng_price = command["price"];
            ng_remain = command["remain"];
            ng_desc = command["desc"];
            ng_type_s = command["goodsType"];
            ng_type = type_map[ng_type_s];
            u->addGoods(ng_type, ng_price, ng_name, ng_desc, ng_remain);
            callback["error"] = nullptr;
            callback["goodsName"] = ng_name;
            callback["goodsType"] = ng_type_s;
            callback["desc"] = ng_desc;
            callback["remain"] = ng_remain;
            callback["price"] = ng_price;
            callback["seller"] = u->getUserName();
        }
        //改成广播
        for(int i=0; i < clientList.size(); i ++) {
            send_callback(callback, i);
        }
        break;
    }
    case CMD_DEL_GOODS: {
        std::string g_name;
        uint64_t uid;
        g_name = command["goodsName"];
        uid = command["uid"];
        User *u = id_to_user[uid];
        callback["type"] = CB_DEL_GOODS;
        if(goods_list[std::pair(g_name, uid)] == nullptr) {
            callback["error"] = "no such goods";
        } else {
            u->deleteGoods(g_name);
            deleteGoods(g_name, uid);
            id--;
            callback["error"] = nullptr;
        }
        //改成广播
        for(int i=0; i < clientList.size(); i ++) {
            send_callback(callback, i);
        }
        break;
    }
    case CMD_CHG_REMAIN: {
        std::string g_name;
        uint64_t uid;
        g_name = command["goodsName"];
        uid = command["uid"];
        User *u = id_to_user[uid];
        uint64_t g_remain;
        g_remain = command["remain"];
        callback["type"] = CB_CHG_REMAIN;
        if(goods_list[std::pair(g_name, uid)] == nullptr) {
            callback["error"] = "no such goods";
        } else {
            u->manageGoods(g_name, -1, g_remain);
            changeGoods(g_name, uid);
            callback["error"] = nullptr;
        }
        //改成广播
        for(int i=0; i < clientList.size(); i ++) {
            send_callback(callback, i);
        }
        break;
    }
    case CMD_CHG_OPRICE: {
        std::string g_name;
        uint64_t uid;
        g_name = command["goodsName"];
        uid = command["uid"];
        User *u = id_to_user[uid];
        double g_price;
        g_price = command["price"];
        callback["type"] = CB_CHG_OPRICE;
        if(goods_list[std::pair(g_name, uid)] == nullptr) {
            callback["error"] = "no such goods";
        } else {
            u->manageGoods(g_name, g_price, -1);
            changeGoods(g_name, uid);
            callback["error"] = nullptr;
        }
        //改成广播
        for(int i=0; i < clientList.size(); i ++) {
            send_callback(callback, i);
        }
        break;
    }
    case CMD_CHG_DISCOUNT: {
        std::string type_s;
        goods_type_t g_type;
        double discount;
        type_s = command["goodsType"];
        g_type = type_map[type_s];
        discount = command["discount"];
        uint64_t uid = command["uid"];
        User *u = id_to_user[uid];
        discount/=100;
        u->manageDiscount(g_type,discount);
        refreshUserJson(u);
        callback["type"] = CB_CHG_DISCOUNT;
        callback["error"] = nullptr;
        //改成广播
        for(int i=0; i < clientList.size(); i ++) {
            send_callback(callback, i);
        }
        break;
    }
    case CMD_ADD_CART: {
        cartObj item;
        uint64_t uid = command["uid"];
        item.pnu = std::pair<std::string,uint64_t>(command["goodsName"], user_list[command["seller"]]->getUserId());
        item.amount = command["amount"];
        User *u = id_to_user[uid];
        u->addCart(item);
        refreshUserJson(u);
        callback["type"] = CB_ADD_CART;
        callback["error"] = nullptr;
        send_callback(callback, socket_id);
        break;
    }
    case CMD_DEL_CART: {
        cartObj item;
        uint64_t uid = command["uid"];
        User *u = id_to_user[uid];
        item.pnu=std::pair<std::string,uint64_t>(command["goodsName"], user_list[command["seller"]]->getUserId());
        item.amount = command["amount"];
        u->delCart(item);
        refreshUserJson(u);
        callback["type"] = CB_DEL_CART;
        callback["error"] = nullptr;
        send_callback(callback, socket_id);
        break;
    }
    case CMD_PURCHASE: {
        cartObj item;
        uint64_t uid = command["uid"];
        User *u = id_to_user[uid];
        item.pnu=std::pair<std::string,uint64_t>(command["goodsName"], user_list[command["seller"]]->getUserId());
        item.amount = command["amount"];
        callback["type"] = CB_PURCHASE;
        if(!goods_list[item.pnu] || goods_list[item.pnu]->getRemain() == 0) {
            callback["error"] = "goods unavailable";
        } else {
            if(item.amount <= goods_list[item.pnu]->getRemain()) {
                if(item.amount * goods_list[item.pnu]->getPrice() <= u->getBalance()) {
                    u->pay(item.amount * goods_list[item.pnu]->getPrice());
                    id_to_user[item.pnu.second]->recharge(item.amount * goods_list[item.pnu]->getPrice());
                    goods_list[item.pnu]->changeRemain(goods_list[item.pnu]->getRemain() - item.amount);
                    u->delCart(item);
                    refreshUserJson(u);
                    refreshUserJson(id_to_user[item.pnu.second]);
                    changeGoods(item.pnu.first, item.pnu.second);
                    callback["error"] = nullptr;
                    callback["customerUid"] = command["uid"];
                    callback["customerBalance"] = u->getBalance();
                    callback["sellerUid"] = item.pnu.second;
                    callback["sellerBalance"] = id_to_user[item.pnu.second]->getBalance();
                    for(int i=0; i < clientList.size(); i ++) {
                        send_callback(callback, i);
                    }
                    return;
                } else {
                    callback["error"] = "insufficient balance";
                }
            } else {
                uint64_t t_amount = item.amount;
                item.amount = goods_list[item.pnu]->getRemain();
                if(item.amount * goods_list[item.pnu]->getPrice() <= u->getBalance()) {
                    u->pay(item.amount * goods_list[item.pnu]->getPrice());
                    id_to_user[item.pnu.second]->recharge(item.amount * goods_list[item.pnu]->getPrice());
                    item.amount = t_amount - item.amount;
                    goods_list[item.pnu]->changeRemain(0);
                    u->changeCart(item);
                    refreshUserJson(u);
                    refreshUserJson(id_to_user[item.pnu.second]);
                    changeGoods(item.pnu.first,item.pnu.second);
                    callback["error"] = nullptr;
                    callback["customerUid"] = command["uid"];
                    callback["customerBalance"] = u->getBalance();
                    callback["sellerUid"] = item.pnu.second;
                    callback["sellerBalance"] = id_to_user[item.pnu.second]->getBalance();
                    for(int i=0; i < clientList.size(); i ++) {
                        send_callback(callback, i);
                    }
                    return;
                } else {
                    callback["error"] = "insufficient balance";
                }
            }
        }
        send_callback(callback, socket_id);
        break;
    }
    case CMD_RE_CART: {
        uint64_t uid = command["uid"];
        User *u = id_to_user[uid];
        auto cart = u->getCart();
        callback["type"] = CB_RE_CART;
        callback["cart"] = json::array();
        callback["error"] = nullptr;
        for(uint64_t i=0;i<cart.size();i++) {
            auto pnu = cart[i].pnu;
            uint64_t amount = cart[i].amount;
            auto cart_item = goods_list[pnu];
            std::string type_s;
            switch (cart_item->getType()) {
            case food:
                type_s = "食品";
                break;
            case cloths:
                type_s = "服装";
                break;
            case book:
                type_s = "书籍";
                break;
            case electronic:
                type_s = "电子产品";
                break;
            }
            json item;
            item["goodsName"] = cart_item->getName();
            item["goodsType"] = type_s;
            item["price"] = cart_item->getPrice();
            item["seller"] = id_to_user[pnu.second]->getUserName();
            item["amount"] = amount;
            callback["cart"].push_back(item);
        }
        send_callback(callback, socket_id);
        break;
    }
    case CMD_RE_GOODS: {
        callback["type"] = CB_RE_GOODS;
        callback["goods"] = json::array();
        for(uint64_t i=0;i<goods_name.size();i++) {
            std::string type_s;
            switch (goods_list[goods_name[i]]->getType()) {
            case food:
                type_s = "食品";
                break;
            case cloths:
                type_s = "服装";
                break;
            case book:
                type_s = "书籍";
                break;
            case electronic:
                type_s = "电子产品";
                break;
            }
            json item;
            item["goodsName"] = goods_name[i].first;
            item["goodsType"] = type_s;
            item["desc"] = goods_list[goods_name[i]]->getDesc();
            item["price"] = goods_list[goods_name[i]]->getPrice();
            item["remain"] = goods_list[goods_name[i]]->getRemain();
            item["seller"] = id_to_user[goods_name[i].second]->getUserName();
            callback["goods"].push_back(item);
        }
        callback["error"] = nullptr;
        QElapsedTimer time;
        time.start();
        while(time.elapsed() < 3000)             //等待时间流逝3秒钟
            QCoreApplication::processEvents();
        send_callback(callback, socket_id);
        break;
    }
    case CMD_CHG_CART: {
        uint64_t uid = command["uid"];
        User *u = id_to_user[uid];
        cartObj item;
        item.pnu=std::pair<std::string,uint64_t>(command["goodsName"], user_list[command["seller"]]->getUserId());
        item.amount = command["amount"];
        u->changeCart(item);
        refreshUserJson(u);
        json callback;
        callback["type"] = CB_CHG_CART;
        callback["error"] = nullptr;
        send_callback(callback, socket_id);
        break;
    }
    case CMD_RECHARGE: {
        uint64_t uid = command["uid"];
        double amount = command["amount"];
        User *u = id_to_user[uid];
        u->recharge(amount);
        writeUserConfig();
        json callback;
        callback["type"] = CB_RECHARGE;
        callback["error"] = nullptr;
        callback["balance"] = u->getBalance();
        send_callback(callback, socket_id);
        break;
    }
    default:
        break;
    }
}
