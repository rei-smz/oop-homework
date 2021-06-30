#include "managegoods.h"
#include "ui_managegoods.h"
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
    if(userType != merchant) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","当前无用户登录或用户类别不为商家",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        this->close();
        return;
    }
    std::string g_name;
    g_name=ui->nameLine->text().toStdString();
    using namespace nlohmann;
    json sendCmd;
    sendCmd["type"] = CMD_DEL_GOODS;
    sendCmd["uid"] = userId;
    sendCmd["goodsName"] = g_name;
    sendMsg(sendCmd);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
}

void manageGoods::on_pushButton_2_clicked()
{
    if(userType != merchant) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","当前无用户登录或用户类别不为商家",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        this->close();
        return;
    }
    std::string g_name;
    g_name = ui->nameLine->text().toStdString();
    uint64_t g_remain;
    bool success;
    g_remain = ui->remainLine->text().toULongLong(&success);
    if(!success) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","输入的数值不合法",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    }
    using namespace nlohmann;
    json sendCmd;
    sendCmd["type"] = CMD_CHG_REMAIN;
    sendCmd["goodsName"] = g_name;
    sendCmd["uid"] = userId;
    sendCmd["remain"] = g_remain;
    sendMsg(sendCmd);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
}

void manageGoods::on_pushButton_4_clicked()
{
    if(userType != merchant) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","当前无用户登录或用户类别不为商家",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        this->close();
        return;
    }
    std::string g_name;
    g_name = ui->nameLine->text().toStdString();
    double g_price;
    bool success;
    g_price = ui->priceLine->text().toDouble(&success);
    if(!success||g_price<0) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","输入的数值不合法",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    }
    using namespace nlohmann;
    json sendCmd;
    sendCmd["type"] = CMD_CHG_OPRICE;
    sendCmd["goodsName"] = g_name;
    sendCmd["uid"] = userId;
    sendCmd["price"] = g_price;
    sendMsg(sendCmd);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
}

void manageGoods::on_pushButton_3_clicked()
{
    if(userType != merchant) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","当前无用户登录或用户类别不为商家",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        this->close();
        return;
    }
    std::string type_s;
    double discount;
    bool success;
    type_s = ui->comboBox->currentText().toStdString();
    discount = ui->discountLine->text().toDouble(&success);
    if(!success||(discount<0||discount>100)) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","输入到数值不合法",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    } else {
        using namespace nlohmann;
        json sendCmd;
        sendCmd["type"] = CMD_CHG_DISCOUNT;
        sendCmd["goodsType"] = type_s;
        sendCmd["uid"] = userId;
        sendCmd["discount"] = discount;
        sendMsg(sendCmd);
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->pushButton_4->setEnabled(false);

    }
}

void manageGoods::checkDelCallback(bool s) {
    if(s == true) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Information,"成功","商品已删除",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    } else {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","未找到该商品",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    }
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_4->setEnabled(true);
}

void manageGoods::checkRemainCallback(bool s) {
    if(s == true) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Information,"成功","商品余量已修改",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    } else {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","未找到该商品",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    }
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_4->setEnabled(true);
}

void manageGoods::checkOPriceCallback(bool s) {
    if(s == true) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Information,"成功","商品原价已修改",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    } else {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","未找到该商品",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    }
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_4->setEnabled(true);
}

void manageGoods::checkDiscountCallback(bool s) {
    if(s == true) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Information,"成功","折扣已修改",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    } else {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"失败","折扣修改失败",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    }
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_4->setEnabled(true);
}

void manageGoods::closeWindow() {
    this->close();
}
