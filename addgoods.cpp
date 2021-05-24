#include "addgoods.h"
#include "ui_addgoods.h"
#include "management.h"
#include "db.h"
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
    if(!current_user||current_user->getUserType()!=merchant) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","当前无用户登录或用户类别不为商家",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        this->close();
        return;
    }
    std::string ng_name,ng_desc,ng_type_s;
    goods_type_t ng_type;
    double ng_price;
    uint64_t ng_remain;
    ng_name = ui->nameLine->text().toStdString();
    if(goods_list[std::pair(ng_name,current_user->getUserId())]!=nullptr) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","不能添加重复名称的商品",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    } else {
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
            ng_type = type_map[ng_type_s];
            current_user->addGoods(ng_type,ng_price,ng_name,ng_desc,ng_remain);
            QMessageBox* msg;
            msg = new QMessageBox(QMessageBox::Information,"成功","添加商品成功",QMessageBox::Ok|QMessageBox::Default);
            msg->show();
            insertGoods();
            emit sendOk();
        }
    }
}

void addGoods::closeWindow() {
    this->close();
}
