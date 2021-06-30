#include "account.h"
#include "ui_account.h"
#include <QMessageBox>

account::account(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::account)
{
    ui->setupUi(this);
    connect(ui->passLine,SIGNAL(returnPressed()),ui->pushButton,SIGNAL(clicked()),Qt::UniqueConnection);
}

account::~account()
{
    delete ui;
}

void account::on_pushButton_clicked()
{
    std::string t_name,t_pass;
    t_name = ui->userLine->text().toStdString();
    t_pass = ui->passLine->text().toStdString();
    using namespace nlohmann;
    json sendCmd;
    sendCmd["type"] = CMD_LOGIN;
    sendCmd["username"] = t_name;
    sendCmd["password"] = t_pass;
    sendMsg(sendCmd);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
}

void account::on_pushButton_2_clicked()
{
    std::string t_name,t_pass,type_s;
    t_name = ui->userLine->text().toStdString();
    t_pass = ui->passLine->text().toStdString();
    type_s = ui->comboBox->currentText().toStdString();
    using namespace nlohmann;
    json sendCmd;
    sendCmd["type"] = CMD_REG;
    sendCmd["username"] = t_name;
    sendCmd["password"] = t_pass;
    sendCmd["userType"] = type_s;
    sendMsg(sendCmd);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
}

void account::checkLoginCallback(const QString err) {
    if(err == "") {
        QMessageBox* msg;
        qDebug()<<"登录成功";
        msg = new QMessageBox(QMessageBox::Information,"成功","登录成功",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        this->close();
    } else if(err == "cannot find this user") {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","用户未找到",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        qDebug()<<"用户未找到";
    } else {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","密码错误",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        qDebug()<<"密码错误";
    }
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
}

void account::checkRegCallback(const QString err) {
    if(err == "") {
        QMessageBox* msg;
        qDebug()<<"用户注册成功";
        msg = new QMessageBox(QMessageBox::Information,"成功","注册成功",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        this->close();
    } else {
        QMessageBox* msg;
        qDebug()<<"用户已存在";
        msg = new QMessageBox(QMessageBox::Warning,"警告","用户已存在",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    }
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
}
