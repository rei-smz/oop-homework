#include "managegoods.h"
#include "ui_managegoods.h"
#include "management.h"
#include "db.h"
#include <QMessageBox>

manageGoods::manageGoods(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::manageGoods)
{
    ui->setupUi(this);
}

manageGoods::~manageGoods()
{
    delete ui;
}

void manageGoods::on_pushButton_clicked()
{
    if(!current_user||current_user->getUserType()!=merchant) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","当前无用户登录或用户类别不为商家",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        this->close();
        return;
    }
    std::string g_name;
    g_name=ui->nameLine->text().toStdString();
    if(goods_list[std::pair(g_name,current_user->getUserId())]==nullptr) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","未找到该商品",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    } else {
        current_user->deleteGoods(g_name);
        deleteGoods(g_name,current_user->getUserId());
        id--;
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Information,"成功","商品已删除",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        emit sendOk();
    }
}

void manageGoods::on_pushButton_2_clicked()
{
    if(!current_user||current_user->getUserType()!=merchant) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","当前无用户登录或用户类别不为商家",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        this->close();
        return;
    }
    std::string g_name;
    g_name = ui->nameLine->text().toStdString();
    if(goods_list[std::pair(g_name,current_user->getUserId())]==nullptr) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","未找到该商品",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    } else {
        uint64_t g_remain;
        bool success;
        g_remain = ui->remainLine->text().toULongLong(&success);
        if(!success) {
            QMessageBox* msg;
            msg = new QMessageBox(QMessageBox::Critical,"错误","输入的数值不合法",QMessageBox::Ok|QMessageBox::Default);
            msg->show();
        } else {
            current_user->manageGoods(g_name,-1,g_remain);
            changeGoods(g_name,current_user->getUserId());
            QMessageBox* msg;
            msg = new QMessageBox(QMessageBox::Information,"成功","商品余量已修改",QMessageBox::Ok|QMessageBox::Default);
            msg->show();
            emit sendOk();
        }
    }
}

void manageGoods::on_pushButton_4_clicked()
{
    if(!current_user||current_user->getUserType()!=merchant) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","当前无用户登录或用户类别不为商家",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        this->close();
        return;
    }
    std::string g_name;
    g_name = ui->nameLine->text().toStdString();
    if(goods_list[std::pair(g_name,current_user->getUserId())]==nullptr) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","未找到该商品",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    } else {
        double g_price;
        bool success;
        g_price = ui->priceLine->text().toDouble(&success);
        if(!success||g_price<0) {
            QMessageBox* msg;
            msg = new QMessageBox(QMessageBox::Critical,"错误","输入的数值不合法",QMessageBox::Ok|QMessageBox::Default);
            msg->show();
        } else {
            current_user->manageGoods(g_name,g_price,-1);
            changeGoods(g_name,current_user->getUserId());
            QMessageBox* msg;
            msg = new QMessageBox(QMessageBox::Information,"成功","商品原价已修改",QMessageBox::Ok|QMessageBox::Default);
            msg->show();
            emit sendOk();
        }
    }
}

void manageGoods::on_pushButton_3_clicked()
{
    if(!current_user||current_user->getUserType()!=merchant) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","当前无用户登录或用户类别不为商家",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        this->close();
        return;
    }
    std::string type_s;
    goods_type_t g_type;
    double discount;
    bool success;
    type_s = ui->comboBox->currentText().toStdString();
    g_type = type_map[type_s];
    discount = ui->discountLine->text().toDouble(&success);
    if(!success||(discount<0||discount>100)) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","输入到数值不合法",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    } else {
        discount/=100;
        current_user->manageDiscount(g_type,discount);
        refreshUserJson(current_user);
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Information,"成功","折扣已修改",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        emit sendOk();
    }
}

void manageGoods::closeWindow() {
    this->close();
}
