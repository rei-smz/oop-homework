#include "changepass.h"
#include "ui_changepass.h"
#include "management.h"
#include "file_io.h"
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
    if(!current_user) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","当前无用户登录",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        this->close();
        return;
    }
    std::string old_pass,new_pass;
    old_pass = ui->oldLine->text().toStdString();
    new_pass = ui->newLine->text().toStdString();
    if(old_pass!=current_user->getUserPass()) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","旧密码输入错误",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    } else if(old_pass==new_pass) {
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Critical,"错误","新旧密码相同",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
    } else {
        current_user->changePassword(old_pass,new_pass);
        for(auto& u_obj:user_json_list) {
            if(u_obj["name"]==current_user->getUserName()) {
                u_obj["password"]=current_user->getUserPass();
            }
        }
        writeUserConfig();
        QMessageBox* msg;
        msg = new QMessageBox(QMessageBox::Information,"成功","密码修改成功",QMessageBox::Ok|QMessageBox::Default);
        msg->show();
        this->close();
    }

}
