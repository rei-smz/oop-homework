#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "account.h"
#include "changepass.h"
#include "recharge.h"
#include "addgoods.h"
#include "managegoods.h"
#include "management.h"
#include <QTableWidget>
#include <QTableWidgetItem>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    connect(account_window,SIGNAL(sendOk()),this,SLOT(refresh_user()));
    account_window->show();
}

void MainWindow::on_xiugaimima_triggered()
{
    //打开修改密码窗口
    changepass* chg_pass_window;
    chg_pass_window = new changepass;
    chg_pass_window->show();
}

void MainWindow::on_actiontuichu_triggered()
{
    //退出登录
    current_user = nullptr;
    refresh_user();
}

void MainWindow::refresh_goods()
{
    int64_t rows = goods_name.size();
    ui->tableWidget->setRowCount(rows);
    for(int64_t r=0;r<rows;r++) {
        std::string type_s;
        ui->tableWidget->setItem(r,0,new QTableWidgetItem(QString::fromStdString(goods_name[r].first)));
        switch (goods_list[goods_name[r]]->getType()) {
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
        ui->tableWidget->setItem(r,1,new QTableWidgetItem(QString::fromStdString(type_s)));
        ui->tableWidget->setItem(r,2,new QTableWidgetItem(QString::fromStdString(goods_list[goods_name[r]]->getDesc())));
        ui->tableWidget->setItem(r,3,new QTableWidgetItem(QString("%1").arg(goods_list[goods_name[r]]->getPrice())));
        ui->tableWidget->setItem(r,4,new QTableWidgetItem(QString("%1").arg(goods_list[goods_name[r]]->getRemain())));
        ui->tableWidget->setItem(r,5,new QTableWidgetItem(QString::fromStdString(id_to_user[goods_name[r].second]->getUserName())));
    }
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
}

void MainWindow::append_goods()
{
    int64_t rows = goods_name.size();
    ui->tableWidget->setRowCount(rows);
    std::string type_s;
    ui->tableWidget->setItem(rows-1,0,new QTableWidgetItem(QString::fromStdString(goods_name[rows-1].first)));
    switch (goods_list[goods_name[rows-1]]->getType()) {
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
    ui->tableWidget->setItem(rows-1,1,new QTableWidgetItem(QString::fromStdString(type_s)));
    ui->tableWidget->setItem(rows-1,2,new QTableWidgetItem(QString::fromStdString(goods_list[goods_name[rows-1]]->getDesc())));
    ui->tableWidget->setItem(rows-1,3,new QTableWidgetItem(QString("%1").arg(goods_list[goods_name[rows-1]]->getPrice())));
    ui->tableWidget->setItem(rows-1,4,new QTableWidgetItem(QString("%1").arg(goods_list[goods_name[rows-1]]->getRemain())));
    ui->tableWidget->setItem(rows-1,5,new QTableWidgetItem(QString::fromStdString(id_to_user[goods_name[rows-1].second]->getUserName())));
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
}

void MainWindow::refresh_user()
{
    if(current_user==nullptr) {
        ui->currentUserLabel->setText("当前用户：未登录");
        ui->xiugaimima->setEnabled(false);
        ui->actiontuichu->setEnabled(false);
        ui->actionaddGoods->setEnabled(false);
        ui->actionmanage->setEnabled(false);
        ui->actionrecharge->setEnabled(false);
        ui->actioncart->setEnabled(false);
        ui->balanceLabel->setText("");
    } else {
        ui->currentUserLabel->setText(QString("当前用户：")+QString::fromStdString(current_user->getUserName()));
        ui->xiugaimima->setEnabled(true);
        ui->actiontuichu->setEnabled(true);
        ui->actionrecharge->setEnabled(true);
        ui->balanceLabel->setText(QString("账户余额：%1元").arg(current_user->getBalance()));
        if(current_user->getUserType()==merchant) {
            ui->actionaddGoods->setEnabled(true);
            ui->actionmanage->setEnabled(true);
        } else {
            ui->actioncart->setEnabled(true);
        }
    }
}

void MainWindow::on_actionrecharge_triggered()
{
    recharge_w* rchg_window;
    rchg_window = new recharge_w;
    connect(rchg_window,SIGNAL(sendOk()),this,SLOT(refresh_user()));
    rchg_window->show();
}

void MainWindow::on_actionaddGoods_triggered()
{
    addGoods* ag_window;
    ag_window = new addGoods;
    connect(ag_window,SIGNAL(sendOk()),this,SLOT(append_goods()));
    ag_window->show();
}

void MainWindow::on_actionmanage_triggered()
{
    manageGoods* mg_window;
    mg_window = new manageGoods;
    connect(mg_window,SIGNAL(sendOk()),this,SLOT(refresh_goods()));
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
