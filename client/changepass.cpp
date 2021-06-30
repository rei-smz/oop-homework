#include "changepass.h"
#include "ui_changepass.h"
#include <QMessageBox>

changepass::changepass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::changepass)
{
    ui->setupUi(this);
}

changepass::~changepass()
{
    delete ui;
}

void changepass::on_pushButton_2_clicked()
{
    if(userType == guest) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","当前无用户登录",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        this->close();
        return;
    }
    std::string old_pass,new_pass;
    old_pass = ui->oldLine->text().toStdString();
    new_pass = ui->newLine->text().toStdString();
    using namespace nlohmann;
    json sendCmd;
    sendCmd["type"] = CMD_CHG_PASS;
    sendCmd["uid"] = userId;
    sendCmd["oldPass"] = old_pass;
    sendCmd["newPass"] = new_pass;
    sendMsg(sendCmd);
    ui->pushButton_2->setEnabled(false);
}

void changepass::closeWindow() {
    this->close();
}

void changepass::checkCallback(const QString err) {
    if(err == "") {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Information,"成功","密码修改成功",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        this->close();
    } else if(err == "wrong password") {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","旧密码输入错误",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    } else {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","新旧密码相同",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    }
    ui->pushButton_2->setEnabled(true);
}
