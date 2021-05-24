#include "account.h"
#include "ui_account.h"
#include "management.h"
#include "file_io.h"
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
    auto u=user_list[t_name];
    if(u!=nullptr) {
        if(t_pass==u->getUserPass()) {
            current_user=u;
            QMessageBox* msg;
            qDebug()<<"登录成功";
            msg = new QMessageBox(QMessageBox::Information,"成功","登录成功",QMessageBox::Ok|QMessageBox::Default);
            msg->show();
            emit sendOk();
            this->close();
        } else {
            QMessageBox* msg;
            msg = new QMessageBox(QMessageBox::Critical,"错误","密码错误",QMessageBox::Ok|QMessageBox::Default);
            msg->show();
            qDebug()<<"密码错误";
        }
        return;
    }
    QMessageBox* msg;
    msg = new QMessageBox(QMessageBox::Critical,"错误","用户未找到",QMessageBox::Ok|QMessageBox::Default);
    msg->show();
    qDebug()<<"用户未找到";
}

void account::on_pushButton_2_clicked()
{
    std::string t_name,t_pass,type_s;
    user_type_t type;
    t_name = ui->userLine->text().toStdString();
    t_pass = ui->passLine->text().toStdString();
    type_s = ui->comboBox->currentText().toStdString();
    auto u=user_list[t_name];
    if(u!=nullptr) {
        QMessageBox* msg;
        qDebug()<<"用户已存在";
        msg = new QMessageBox(QMessageBox::Warning,"警告","用户已存在",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        return;
    }
    user_count++;
    uint64_t uid = user_count-1;
    if(type_s=="商家") {
        type=merchant;
    } else {
        type=customer;
    }
    User* nu;
    switch (type) {
        case merchant:
            nu=new Merchant(uid,t_name,t_pass,0);
            break;
        case customer:
            nu=new Customer(uid,t_name,t_pass,0);
            break;
    }
    user_list[t_name]=nu;
    id_to_user[uid]=nu;
    current_user=nu;
    changeUserToJson(nu);
    writeUserConfig();
    QMessageBox* msg;
    qDebug()<<"用户注册成功";
    msg = new QMessageBox(QMessageBox::Information,"成功","注册成功",QMessageBox::Ok|QMessageBox::Default);
    msg->show();
    emit sendOk();
    this->close();
}
