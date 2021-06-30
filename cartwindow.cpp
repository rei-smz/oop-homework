#include "cartwindow.h"
#include "ui_cartwindow.h"
#include "management.h"
#include "db.h"
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
    if(!current_user||current_user->getUserType()!=customer) {
        return;
    }
    int64_t row_index = ui->tableWidget->currentRow();
    if(row_index == -1) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","未选中商品",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        return;
    }
    cartObj item;
    item.pnu=std::pair<std::string,uint64_t>(ui->tableWidget->item(row_index,0)->text().toStdString(),user_list[ui->tableWidget->item(row_index,3)->text().toStdString()]->getUserId());
    item.amount = ui->tableWidget->item(row_index,4)->text().toULongLong();
    current_user->delCart(item);
    refreshUserJson(current_user);
    refresh_cart();
    QMessageBox* msg;
    msg = new QMessageBox(QMessageBox::Information,"成功","已删除该商品",QMessageBox::Ok|QMessageBox::Default);
    msg->show();
}

void cartWindow::on_purchaseButton_clicked()
{
    if(!current_user||current_user->getUserType()!=customer) {
        return;
    }
    int64_t row_index = ui->tableWidget->currentRow();
    if(row_index == -1) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","未选中商品",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        return;
    }
    cartObj item;
    item.pnu=std::pair<std::string,uint64_t>(ui->tableWidget->item(row_index,0)->text().toStdString(),user_list[ui->tableWidget->item(row_index,3)->text().toStdString()]->getUserId());
    item.amount = ui->tableWidget->item(row_index,4)->text().toULongLong();
    if(!goods_list[item.pnu] || goods_list[item.pnu]->getRemain() == 0) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","选定的商品当前不可购买",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        return;
    }
    bill *bill_window;
    bill_window = new bill;
    QLabel *info = bill_window->findChild<QLabel *>("label");
    if(item.amount <= goods_list[item.pnu]->getRemain()) {
        info->setText(QString("您的订单如下：\n") + QString::fromStdString(item.pnu.first) + QString("\n共%1件").arg(item.amount));
        goods_list[item.pnu]->changeRemain(goods_list[item.pnu]->getRemain() - item.amount);
        if(bill_window->exec() == QDialog::Accepted) {
            if(item.amount * goods_list[item.pnu]->getPrice() <= current_user->getBalance()) {
                current_user->pay(item.amount * goods_list[item.pnu]->getPrice());
                id_to_user[item.pnu.second]->recharge(item.amount * goods_list[item.pnu]->getPrice());
                current_user->delCart(item);
                refreshUserJson(current_user);
                refreshUserJson(id_to_user[item.pnu.second]);
                changeGoods(item.pnu.first,item.pnu.second);
                emit sendPayOk();
                refresh_cart();
                QMessageBox* msg;
                msg = new QMessageBox(QMessageBox::Information,"成功","购买成功",QMessageBox::Ok|QMessageBox::Default);
                msg->show();
            } else {
                goods_list[item.pnu]->changeRemain(goods_list[item.pnu]->getRemain() + item.amount);
                QMessageBox* msg;
                msg = new QMessageBox(QMessageBox::Critical,"错误","余额不足",QMessageBox::Ok|QMessageBox::Default);
                msg->show();
            }
        } else {
            goods_list[item.pnu]->changeRemain(goods_list[item.pnu]->getRemain() + item.amount);
            QMessageBox* msg;
            msg = new QMessageBox(QMessageBox::Information,"提示","订单已取消",QMessageBox::Ok|QMessageBox::Default);
            msg->show();
        }
    } else {
        uint64_t t_amount = item.amount;
        item.amount = goods_list[item.pnu]->getRemain();
        info->setText(QString("您的订单如下：\n") + QString::fromStdString(item.pnu.first) + QString("\n共%1件").arg(item.amount));
        goods_list[item.pnu]->changeRemain(0);
        if(bill_window->exec() == QDialog::Accepted) {
            if(item.amount * goods_list[item.pnu]->getPrice() <= current_user->getBalance()) {
                current_user->pay(item.amount * goods_list[item.pnu]->getPrice());
                id_to_user[item.pnu.second]->recharge(item.amount * goods_list[item.pnu]->getPrice());
                item.amount = t_amount - item.amount;
                current_user->changeCart(item);
                refreshUserJson(current_user);
                refreshUserJson(id_to_user[item.pnu.second]);
                changeGoods(item.pnu.first,item.pnu.second);
                emit sendPayOk();
                refresh_cart();
                QMessageBox* msg;
                msg = new QMessageBox(QMessageBox::Information,"成功","选定商品的数量超过该商品的余量，已为您购买最大可购买数量",QMessageBox::Ok|QMessageBox::Default);
                msg->show();
            } else {
                goods_list[item.pnu]->changeRemain(item.amount);
                QMessageBox* msg;
                msg = new QMessageBox(QMessageBox::Critical,"错误","余额不足",QMessageBox::Ok|QMessageBox::Default);
                msg->show();
            }
        } else {
            goods_list[item.pnu]->changeRemain(item.amount);
            QMessageBox* msg;
            msg = new QMessageBox(QMessageBox::Information,"提示","订单已取消",QMessageBox::Ok|QMessageBox::Default);
            msg->show();
        }
    }
}

void cartWindow::refresh_cart() {
    if(!current_user||current_user->getUserType()!=customer) {
        return;
    }
    auto user_cart = current_user->getCart();
    int64_t rows = user_cart.size();
    ui->tableWidget->setRowCount(rows);
    for(int64_t r=0;r<rows;r++) {
        auto pnu = user_cart[r].pnu;
        uint64_t amount = user_cart[r].amount;
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
        ui->tableWidget->setItem(r,0,new QTableWidgetItem(QString::fromStdString(pnu.first)));
        ui->tableWidget->setItem(r,1,new QTableWidgetItem(QString::fromStdString(type_s)));
        ui->tableWidget->setItem(r,2,new QTableWidgetItem(QString("%1").arg(cart_item->getPrice())));
        ui->tableWidget->setItem(r,3,new QTableWidgetItem(QString::fromStdString(id_to_user[pnu.second]->getUserName())));
        ui->tableWidget->setItem(r,4,new QTableWidgetItem(QString("%1").arg(amount)));
        //名称、种类、售价、商家、数量
    }
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
}

void cartWindow::closeWindow() {
    this->close();
}

void cartWindow::on_changeButton_clicked()
{
    if(!current_user||current_user->getUserType()!=customer) {
        return;
    }
    int64_t row_index = ui->tableWidget->currentRow();
    if(row_index == -1) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","未选中商品",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        return;
    }
    cartObj item;
    bool success;
    item.pnu=std::pair<std::string,uint64_t>(ui->tableWidget->item(row_index,0)->text().toStdString(),user_list[ui->tableWidget->item(row_index,3)->text().toStdString()]->getUserId());
    item.amount = ui->lineEdit->text().toULongLong(&success);
    if(!success||item.amount<=0||item.amount>goods_list[item.pnu]->getRemain()) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","输入的数量不合法",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        return;
    }
    current_user->changeCart(item);
    refreshUserJson(current_user);
    refresh_cart();
    QMessageBox* msg;
    msg = new QMessageBox(QMessageBox::Information,"成功","商品数量已修改",QMessageBox::Ok|QMessageBox::Default);
    msg->show();
}
