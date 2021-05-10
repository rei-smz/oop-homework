//
// Created by Rin Shimizu on 18/4/21.
//
#include "file_io.h"
#include "management.h"
#include <QDebug>

using namespace std;

void readUserConfig() {
    ifstream config_file;
    config_file.open("../../../config.json");
    if(!config_file.is_open()) {
        qWarning()<<"打开用户配置文件失败";
        exit(1);
    }
    config_file>>user_json_list;
    for(auto u_obj:user_json_list) {
        User* tu;
        int t_type=u_obj["type"];
        std::string t_name=u_obj["name"],t_pass=u_obj["password"];
        double t_balance=u_obj["balance"];
        uint64_t uid=u_obj["user_id"];
        user_count ++;
        switch (t_type) {
            case 0:
                tu=new Merchant(uid,t_name,t_pass,t_balance);
                user_list[t_name]=tu;
                break;
            case 1:
                tu=new Customer(uid,t_name,t_pass,t_balance);
                user_list[t_name]=tu;
                break;
            default:
                break;
        }
    }
    config_file.close();
}

void writeUserConfig() {
    ofstream config_file;
    config_file.open("../../../config.json");
    if(!config_file.is_open()) {
        qWarning()<<"打开用户配置文件失败";
        return;
    }
    config_file<<user_json_list;
    config_file.close();
}
