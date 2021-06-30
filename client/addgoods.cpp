#include "addgoods.h"
#include "ui_addgoods.h"
#include <QMessageBox>

addGoods::addGoods(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addGoods)
{
    ui->setupUi(this);
}

addGoods::~addGoods()
{
    delete ui;
}

void addGoods::on_pushButton_clicked()
{
    if(userType!=merchant) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","当前无用户登录或用户类别不为商家",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        this->close();
        return;
    }
    std::string ng_name,ng_desc,ng_type_s;
    double ng_price;
    uint64_t ng_remain;
    ng_name = ui->nameLine->text().toStdString();
    bool success1,success2;
    ng_price = ui->priceLine->text().toDouble(&success1);
    ng_remain = ui->remainLine->text().toULongLong(&success2);
    if(!success1||ng_price<0) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","输入的价格不合法",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    } else if(!success2||ng_remain<0) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","输入的余量不合法",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    } else {
        ng_desc = ui->descEdit->toPlainText().toStdString();
        ng_type_s = ui->typeCombo->currentText().toStdString();
        using namespace nlohmann;
        json sendCmd;
        sendCmd["type"] = CMD_ADD_GOODS;
        sendCmd["goodsName"] = ng_name;
        sendCmd["uid"] = userId;
        sendCmd["price"] = ng_price;
        sendCmd["remain"] = ng_remain;
        sendCmd["desc"] = ng_desc;
        sendCmd["goodsType"] = ng_type_s;
        sendMsg(sendCmd);
        ui->pushButton->setEnabled(false);
    }
}

void addGoods::closeWindow() {
    this->close();
}

void addGoods::checkCallback(bool s) {
    if(s) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Information,"成功","添加商品成功",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    } else {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"失败","添加商品失败",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    }
    ui->pushButton->setEnabled(true);
}
