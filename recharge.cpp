#include "recharge.h"
#include "ui_recharge.h"
#include "management.h"
#include "file_io.h"
#include <QMessageBox>
#include <QFileInfo>

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
    bool success;
    amount = ui->moneyLine->text().toDouble(&success);
    if(!success||amount<=0) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","输入到数值不合法",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    } else if(amount>=1000000) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Warning,"提示","系统检测到您的充值金额过大，请向开发者支付10000元以开启此功能，是否开启此功能？",QMessageBox::Ok|QMessageBox::Default);
        if(msg->exec() == QMessageBox::Ok) {
            QLabel *qrwindow = new QLabel(this,Qt::Dialog|Qt::WindowCloseButtonHint);
            QFileInfo qrfile("aaa.jpg");
            if(qrfile.exists()) {
                QImage image;
                image.load("aaa.jpg");
                qrwindow->resize(600,1000);
                qrwindow->setPixmap(QPixmap::fromImage(image.scaled(600,1000,Qt::KeepAspectRatio)));
                qrwindow->show();
            } else {
                qDebug()<<"文件不存在";
            }
        }
    } else {
        current_user->recharge(amount);
        writeUserConfig();
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Information,"成功","充值成功",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        emit sendOk();
        this->close();
    }
}

void recharge_w::closeWindow() {
    this->close();
}
