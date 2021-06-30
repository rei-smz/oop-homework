#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "account.h"
#include "changepass.h"
#include "addgoods.h"
#include "managegoods.h"
#include "cartwindow.h"
#include "recharge.h"
#include "client.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QElapsedTimer>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //connect(&clientSocket, SIGNAL(connected()), this, SLOT());
    connect2Server(QString("127.0.0.1"), 6666);
    connect(&clientSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    refresh_goods();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_dengluzhuce_triggered()
{
    //打开登录注册窗口
    account* account_window;
    account_window = new account;
    connect(this, SIGNAL(sendLoginStatus(const QString)), account_window, SLOT(checkLoginCallback(const QString)));
    connect(this, SIGNAL(sendRegOk(const QString)), account_window, SLOT(checkRegCallback(const QString)));
    account_window->show();
}

void MainWindow::on_xiugaimima_triggered()
{
    //打开修改密码窗口
    changepass* chg_pass_window;
    chg_pass_window = new changepass;
    connect(this, SIGNAL(sendChangePassOk(const QString)), chg_pass_window, SLOT(checkCallback(const QString)));
    connect(this,SIGNAL(sendAccountChanged()),chg_pass_window,SLOT(closeWindow()));
    chg_pass_window->show();
}

void MainWindow::on_actiontuichu_triggered()
{
    //退出登录
//    userType = guest;
    using namespace nlohmann;
    json sendCmd;
    sendCmd["type"] = CMD_LOGOUT;
    sendCmd["uid"] = userId;
    sendMsg(sendCmd);
    ui->xiugaimima->setEnabled(false);
    ui->actiontuichu->setEnabled(false);
    ui->actionaddGoods->setEnabled(false);
    ui->actionmanage->setEnabled(false);
    ui->actionrecharge->setEnabled(false);
    ui->actioncart->setEnabled(false);
    ui->addCartButton->setEnabled(false);
    ui->amountLine->setEnabled(false);
//    emit sendAccountChanged();
//    refresh_user();
}

void MainWindow::refresh_goods()
{
    using namespace nlohmann;
    json sendCmd, callback;
    sendCmd["type"] = CMD_RE_GOODS;
    sendMsg(sendCmd);
}

void MainWindow::refresh_user()
{
    if(userType == guest) {
        ui->currentUserLabel->setText("当前用户：未登录");
        ui->xiugaimima->setEnabled(false);
        ui->actiontuichu->setEnabled(false);
        ui->actionaddGoods->setEnabled(false);
        ui->actionmanage->setEnabled(false);
        ui->actionrecharge->setEnabled(false);
        ui->actioncart->setEnabled(false);
        ui->addCartButton->setEnabled(false);
        ui->amountLine->setEnabled(false);
        ui->balanceLabel->setText("");
    } else {
        ui->currentUserLabel->setText(QString("当前用户：")+QString::fromStdString(username));
        ui->xiugaimima->setEnabled(true);
        ui->actiontuichu->setEnabled(true);
        ui->actionrecharge->setEnabled(true);
        ui->balanceLabel->setText(QString("账户余额：%1元").arg(userBalance,0,'g',20));
        if(userType == merchant) {
            ui->actionaddGoods->setEnabled(true);
            ui->actionmanage->setEnabled(true);
            ui->actioncart->setEnabled(false);
            ui->addCartButton->setEnabled(false);
            ui->amountLine->setEnabled(false);
        } else {
            ui->actioncart->setEnabled(true);
            ui->addCartButton->setEnabled(true);
            ui->amountLine->setEnabled(true);
            ui->actionaddGoods->setEnabled(false);
            ui->actionmanage->setEnabled(false);
        }
    }
}

void MainWindow::on_actionrecharge_triggered()
{
    recharge_w* rchg_window;
    rchg_window = new recharge_w;
    connect(this, SIGNAL(sendRechargeOk(bool)), rchg_window, SLOT(checkCallback(bool)));
    connect(this,SIGNAL(sendAccountChanged()),rchg_window,SLOT(closeWindow()));
    rchg_window->show();
}

void MainWindow::on_actionaddGoods_triggered()
{
    addGoods* ag_window;
    ag_window = new addGoods;
    connect(this, SIGNAL(sendAddGoodsOk(bool)),ag_window, SLOT(checkCallback(bool)));
    connect(this,SIGNAL(sendAccountChanged()),ag_window,SLOT(closeWindow()));
    ag_window->show();
}

void MainWindow::on_actionmanage_triggered()
{
    manageGoods* mg_window;
    mg_window = new manageGoods;
    connect(this, SIGNAL(sendDelGoodsOk(bool)), mg_window, SLOT(checkDelCallback(bool)));
    connect(this, SIGNAL(sendChgRemainOk(bool)), mg_window, SLOT(checkRemainCallback(bool)));
    connect(this, SIGNAL(sendChgOPriceOk(bool)), mg_window, SLOT(checkOPriceCallback(bool)));
    connect(this, SIGNAL(sendChgDiscountOk(bool)), mg_window, SLOT(checkDiscountCallback(bool)));
    connect(this,SIGNAL(sendAccountChanged()),mg_window,SLOT(closeWindow()));
    mg_window->show();
}

void MainWindow::on_pushButton_clicked()
{
    QString keyword;
    int64_t rows=ui->tableWidget->rowCount();
    keyword = ui->filterLine->text();
    if(keyword=="") {
        for(int64_t r=0;r<rows;r++) {
            ui->tableWidget->setRowHidden(r,false);
        }
    } else {
        for(int64_t r=0;r<rows;r++) {
            if(ui->tableWidget->item(r,0)->text().indexOf(keyword)==-1) {
                ui->tableWidget->setRowHidden(r,true);
            }
        }
    }
}

void MainWindow::on_addCartButton_clicked()
{
    int64_t row_index = ui->tableWidget->currentRow();
    if(row_index == -1) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","未选中商品",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        return;
    }
    bool amount_ok;
    uint64_t amount = ui->amountLine->text().toULongLong(&amount_ok);
    if(!amount_ok||amount<=0||amount>ui->tableWidget->item(row_index,4)->text().toULongLong()) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","输入的数量不合法",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        return;
    }
    using namespace nlohmann;
    json sendCmd;
    sendCmd["type"] = CMD_ADD_CART;
    sendCmd["uid"] = userId;
    sendCmd["goodsName"] = ui->tableWidget->item(row_index,0)->text().toStdString();
    sendCmd["seller"] = ui->tableWidget->item(row_index,5)->text().toStdString();
    sendCmd["amount"] = amount;
    sendMsg(sendCmd);
    ui->addCartButton->setEnabled(false);


}

void MainWindow::on_actioncart_triggered()
{
    cartWindow* cart_window;
    cart_window = new cartWindow;
    connect(this,SIGNAL(sendAddCartOk(bool)),cart_window,SLOT(checkCart(bool)));
    connect(this,SIGNAL(sendAccountChanged()),cart_window,SLOT(closeWindow()));
    connect(this, SIGNAL(sendDelCartOk(bool)), cart_window, SLOT(checkDelCallback(bool)));
    connect(this, SIGNAL(sendChangeCartOk(bool)), cart_window, SLOT(checkChangeCallback(bool)));
    connect(this, SIGNAL(sendPurchaseOk(const QString)), cart_window, SLOT(checkPurchaseCallback(const QString)));
    connect(this, SIGNAL(sendReCartData(const QString)), cart_window, SLOT(onReCart(const QString)));
    cart_window->show();
}

void MainWindow::onReadyRead() {
    QByteArray callback = wait4Msg();
    qDebug() << "receive:" << QString(callback);
    using namespace nlohmann;
    json callbackMsg = json::parse(callback.toStdString());
    switch (int(callbackMsg["type"])) {
    case CB_RE_GOODS: {
        if(callbackMsg["error"] == nullptr) {
            auto goodsList = callbackMsg["goods"];
            int64_t rows = goodsList.size();
            ui->tableWidget->setRowCount(rows);
            for(int64_t r=0;r<rows;r++) {
                std::string type_s;
                ui->tableWidget->setItem(r,0,new QTableWidgetItem(QString::fromStdString(goodsList[r]["goodsName"])));
                ui->tableWidget->setItem(r,1,new QTableWidgetItem(QString::fromStdString(goodsList[r]["goodsType"])));
                ui->tableWidget->setItem(r,2,new QTableWidgetItem(QString::fromStdString(goodsList[r]["desc"])));
                ui->tableWidget->setItem(r,3,new QTableWidgetItem(QString("%1").arg(double(goodsList[r]["price"]))));
                ui->tableWidget->setItem(r,4,new QTableWidgetItem(QString("%1").arg(uint64_t(goodsList[r]["remain"]))));
                ui->tableWidget->setItem(r,5,new QTableWidgetItem(QString::fromStdString(goodsList[r]["seller"])));
            }
            ui->tableWidget->resizeColumnsToContents();
            ui->tableWidget->resizeRowsToContents();
        }
        return;
    }
    case CB_ADD_GOODS: {
        if(callbackMsg["error"] == nullptr) {
            int64_t rows = ui->tableWidget->rowCount() + 1;
            ui->tableWidget->setRowCount(rows);
            std::string type_s;
            ui->tableWidget->setItem(rows-1,0,new QTableWidgetItem(QString::fromStdString(callbackMsg["goodsName"])));
            ui->tableWidget->setItem(rows-1,1,new QTableWidgetItem(QString::fromStdString(callbackMsg["goodsType"])));
            ui->tableWidget->setItem(rows-1,2,new QTableWidgetItem(QString::fromStdString(callbackMsg["desc"])));
            ui->tableWidget->setItem(rows-1,3,new QTableWidgetItem(QString("%1").arg(double(callbackMsg["price"]))));
            ui->tableWidget->setItem(rows-1,4,new QTableWidgetItem(QString("%1").arg(uint64_t(callbackMsg["remain"]))));
            ui->tableWidget->setItem(rows-1,5,new QTableWidgetItem(QString::fromStdString(callbackMsg["seller"])));
            ui->tableWidget->resizeColumnsToContents();
            ui->tableWidget->resizeRowsToContents();
            emit sendAddGoodsOk(true);
        } else {
            emit sendAddGoodsOk(false);
        }
        return;
    }
    case CB_REG: {
        if(callbackMsg["error"] == nullptr) {
            userId = callbackMsg["uid"];
            username = callbackMsg["username"];
            if(callbackMsg["userType"] == "商家") {
                userType = merchant;
            } else {
                userType = customer;
            }
            userBalance = 0;
            refresh_user();
            emit sendRegOk("");
        } else {
            emit sendRegOk(QString::fromStdString(callbackMsg["error"]));
        }
        QElapsedTimer time;
        time.start();
        while(time.elapsed() < 1000)
            QCoreApplication::processEvents();
        return;
    }
    case CB_RE_USER: {
        if(callbackMsg["error"] == nullptr) {
            if(callbackMsg["uid"] == -1) {
                userType = guest;
                emit sendAccountChanged();;
                refresh_user();
            } else {
                userId = callbackMsg["uid"];
                username = callbackMsg["username"];
                userBalance = callbackMsg["balance"];
                if(callbackMsg["utype"] == 0) {
                    userType = merchant;
                } else {
                    userType = customer;
                }
                refresh_user();
                emit sendLoginStatus("");
            }
        } else {
            emit sendLoginStatus(QString::fromStdString(callbackMsg["error"]));
        }
        return;
    }
    case CB_CHG_PASS: {
        if(callbackMsg["error"] == nullptr) {
            emit sendChangePassOk("");
        } else {
            emit sendChangePassOk(QString::fromStdString(callbackMsg["error"]));
        }
        return;
    }
    case CB_DEL_GOODS: {
        if(callbackMsg["error"] == nullptr) {
            emit sendDelGoodsOk(true);
            refresh_goods();
        } else {
            emit sendDelGoodsOk(false);
        }
        return;
    }
    case CB_CHG_REMAIN: {
        if(callbackMsg["error"] == nullptr) {
            emit sendChgRemainOk(true);
            refresh_goods();
        } else {
            emit sendChgRemainOk(false);
        }
        return;
    }
    case CB_CHG_OPRICE: {
        if(callbackMsg["error"] == nullptr) {
            emit sendChgOPriceOk(true);
            refresh_goods();
        } else {
            emit sendChgOPriceOk(false);
        }
        return;
    }
    case CB_CHG_DISCOUNT: {
        if(callbackMsg["error"] == nullptr) {
            emit sendChgDiscountOk(true);
            refresh_goods();
        } else {
            emit sendChgDiscountOk(false);
        }
        return;
    }
    case CB_RECHARGE: {
        if(callbackMsg["error"] == nullptr) {
            emit sendRechargeOk(true);
            userBalance = callbackMsg["balance"];
            refresh_user();
        } else {
            emit sendRechargeOk(false);
        }
        return;
    }
    case CB_ADD_CART: {
        if(callbackMsg["error"] == nullptr) {
            emit sendAddCartOk(true);
            ui->tableWidget->setCurrentItem(NULL);
            QMessageBox* msg;
            msg = new QMessageBox(QMessageBox::Information,"成功","加入购物车成功",QMessageBox::Ok|QMessageBox::Default);
            msg->show();
        } else {
            emit sendAddCartOk(false);
            QMessageBox* msg;
            msg = new QMessageBox(QMessageBox::Critical,"失败","加入购物车失败",QMessageBox::Ok|QMessageBox::Default);
            msg->show();
        }
        ui->addCartButton->setEnabled(true);
        return;
    }
    case CB_DEL_CART: {
        if(callbackMsg["error"] == nullptr) {
            emit sendDelCartOk(true);
        } else {
            emit sendDelCartOk(false);
        }
        return;
    }
    case CB_CHG_CART: {
        if(callbackMsg["error"] == nullptr) {
            emit sendChangeCartOk(true);
        } else {
            emit sendChangeCartOk(false);
        }
        return;
    }
    case CB_PURCHASE: {
        if(callbackMsg["error"] != nullptr) { //不成功不会广播
            emit sendPurchaseOk(QString::fromStdString(callbackMsg["error"]));
        } else {
            refresh_goods();
            QElapsedTimer time;
            time.start();
            while(time.elapsed() < 3000)
                QCoreApplication::processEvents();
            if(callbackMsg["customerUid"] == userId) {
                emit sendPurchaseOk("");
                userBalance = callbackMsg["customerBalance"];
                refresh_user();
            }
            if(callbackMsg["sellerUid"] == userId) {
                userBalance = callbackMsg["sellerBalance"];
                refresh_user();
            }
        }
        return;
    }
    case CB_RE_CART: {
        if(callbackMsg["error"] == nullptr) {
            emit sendReCartData(QString::fromStdString(callbackMsg["cart"].dump()));
        }
        return;
    }
    }
}
