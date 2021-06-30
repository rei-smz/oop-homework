#include "cartwindow.h"
#include "ui_cartwindow.h"
#include "bill.h"
#include <QMessageBox>

cartWindow::cartWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cartWindow)
{
    ui->setupUi(this);
    refresh_cart();
}

cartWindow::~cartWindow()
{
    delete ui;
}

void cartWindow::on_delButton_clicked()
{
    if(userType != customer) {
        return;
    }
    int64_t row_index = ui->tableWidget->currentRow();
    if(row_index == -1) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","未选中商品",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        return;
    }
    using namespace nlohmann;
    json sendCmd;
    sendCmd["type"] = CMD_DEL_CART;
    sendCmd["uid"] = userId;
    sendCmd["goodsName"] = ui->tableWidget->item(row_index,0)->text().toStdString();
    sendCmd["seller"] = ui->tableWidget->item(row_index,3)->text().toStdString();
    sendCmd["amount"] = ui->tableWidget->item(row_index,4)->text().toULongLong();
    sendMsg(sendCmd);
    ui->delButton->setEnabled(false);
    ui->changeButton->setEnabled(false);
    ui->purchaseButton->setEnabled(false);
}

void cartWindow::on_purchaseButton_clicked()
{
    if(userType != customer) {
        return;
    }
    int64_t row_index = ui->tableWidget->currentRow();
    if(row_index == -1) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","未选中商品",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        return;
    }

    bill *bill_window;
    bill_window = new bill;
    QLabel *info = bill_window->findChild<QLabel *>("label");
    std::string g_name = ui->tableWidget->item(row_index,0)->text().toStdString();
    std::string seller_name = ui->tableWidget->item(row_index,3)->text().toStdString();
    uint64_t amount = ui->tableWidget->item(row_index,4)->text().toULongLong();
    info->setText(QString("您的订单如下：\n") + QString::fromStdString(g_name) + QString("\n共%1件").arg(amount));
    if(bill_window->exec() == QDialog::Accepted) {
        using namespace nlohmann;
        json sendCmd;
        sendCmd["type"] = CMD_PURCHASE;
        sendCmd["uid"] = userId;
        sendCmd["goodsName"] = g_name;
        sendCmd["seller"] = seller_name;
        sendCmd["amount"] = amount;
        sendMsg(sendCmd);
        ui->delButton->setEnabled(false);
        ui->purchaseButton->setEnabled(false);
        ui->changeButton->setEnabled(false);
    } else {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Information,"提示","订单已取消",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    }
}

void cartWindow::refresh_cart() {
    if(userType != customer) {
        return;
    }
    using namespace nlohmann;
    json sendCmd;
    sendCmd["type"] = CMD_RE_CART;
    sendCmd["uid"] = userId;
    sendMsg(sendCmd);
    ui->changeButton->setEnabled(false);
    ui->delButton->setEnabled(false);
    ui->purchaseButton->setEnabled(false);
}

void cartWindow::closeWindow() {
    this->close();
}

void cartWindow::on_changeButton_clicked()
{
    if(userType != customer) {
        return;
    }
    int64_t row_index = ui->tableWidget->currentRow();
    if(row_index == -1) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","未选中商品",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        return;
    }
    bool success;
    uint64_t new_amount = ui->lineEdit->text().toULongLong(&success);
    if(!success||new_amount<=0) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","输入的数量不合法",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        return;
    }
    std::string g_name = ui->tableWidget->item(row_index,0)->text().toStdString();
    std::string seller_name = ui->tableWidget->item(row_index,3)->text().toStdString();
    using namespace nlohmann;
    json sendCmd;
    sendCmd["type"] = CMD_CHG_CART;
    sendCmd["uid"] = userId;
    sendCmd["goodsName"] = g_name;
    sendCmd["seller"] = seller_name;
    sendCmd["amount"] = new_amount;
    sendMsg(sendCmd);
    ui->changeButton->setEnabled(false);
    ui->delButton->setEnabled(false);
}

void cartWindow::checkCart(bool s) {
    if(s == true) {
        refresh_cart();
    }
}

void cartWindow::checkDelCallback(bool s) {
    if(s == true) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Information,"成功","已删除该商品",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        refresh_cart();
    }
}

void cartWindow::checkPurchaseCallback(const QString err) {
    if(err == "") {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Information,"成功","购买成功",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        refresh_cart();
    } else if(err == "insufficient balance") {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","余额不足",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        ui->changeButton->setEnabled(true);
        ui->delButton->setEnabled(true);
        ui->purchaseButton->setEnabled(true);
    } else {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","选定的商品当前不可购买",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        ui->changeButton->setEnabled(true);
        ui->delButton->setEnabled(true);
        ui->purchaseButton->setEnabled(true);
    }
}

void cartWindow::checkChangeCallback(bool s) {
    if(s == true) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Information,"成功","商品数量已修改",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        refresh_cart();
    }
}

void cartWindow::onReCart(const QString data) {
    using namespace nlohmann;
    json jsonData = json::parse(data.toStdString());
    int64_t rows = jsonData.size();
    ui->tableWidget->setRowCount(rows);
    for(int64_t r=0;r<rows;r++) {
        //auto pnu = data[r].pnu;
        //uint64_t amount = user_cart[r].amount;
        //auto cart_item = goods_list[pnu];
        std::string g_name = jsonData[r]["goodsName"];
        std::string type_s = jsonData[r]["goodsType"];
        double g_price = jsonData[r]["price"];
        uint64_t amount = jsonData[r]["amount"];
        std::string seller_name = jsonData[r]["seller"];
        ui->tableWidget->setItem(r,0,new QTableWidgetItem(QString::fromStdString(g_name)));
        ui->tableWidget->setItem(r,1,new QTableWidgetItem(QString::fromStdString(type_s)));
        ui->tableWidget->setItem(r,2,new QTableWidgetItem(QString("%1").arg(g_price)));
        ui->tableWidget->setItem(r,3,new QTableWidgetItem(QString::fromStdString(seller_name)));
        ui->tableWidget->setItem(r,4,new QTableWidgetItem(QString("%1").arg(amount)));
        //名称、种类、售价、商家、数量
    }
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
    ui->changeButton->setEnabled(true);
    ui->delButton->setEnabled(true);
    ui->purchaseButton->setEnabled(true);
}
