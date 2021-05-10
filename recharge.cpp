#include "recharge.h"
#include "ui_recharge.h"
#include "management.h"
#include "file_io.h"
#include <QMessageBox>

recharge_w::recharge_w(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::recharge_w)
{
    ui->setupUi(this);
}

recharge_w::~recharge_w()
{
    delete ui;
}

void recharge_w::on_rechargeButton_clicked()
{
    double amount;
    amount = ui->moneyLine->text().toDouble();
    current_user->recharge(amount);
    writeUserConfig();
    QMessageBox* msg;
    msg = new QMessageBox(QMessageBox::Information,"成功","充值成功",QMessageBox::Ok|QMessageBox::Default);
    msg->show();
    emit sendOk();
    this->close();
}
