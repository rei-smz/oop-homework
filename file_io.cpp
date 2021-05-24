//
// Created by Rin Shimizu on 18/4/21.
//
#include "file_io.h"
#include "management.h"
#include <QDebug>

using namespace std;

void readUserConfig() {
    ifstream config_file;
    config_file.open("config.json");
    if(!config_file.is_open()) {
        qWarning()<<"打开用户配置文件失败";
        exit(1);
    }
    config_file>>user_json_list;
    User* tu=nullptr;
    for(auto u_obj:user_json_list["Merchant"]) {
        std::string t_name=u_obj["name"],t_pass=u_obj["password"];
        double t_balance=u_obj["balance"];
        uint64_t uid=u_obj["uid"];
        user_count ++;
        using namespace nlohmann;
        json discount;
        tu=new Merchant(uid,t_name,t_pass,t_balance);
        discount=u_obj["discount"];
        tu->manageDiscount(food,discount["food"]);
        tu->manageDiscount(cloths,discount["cloths"]);
        tu->manageDiscount(book,discount["book"]);
        tu->manageDiscount(electronic,discount["electronic"]);
        if(tu) {
            user_list[t_name]=tu;
            id_to_user[uid]=tu;
        }
    }
    for(auto u_obj:user_json_list["Customer"]) {
        std::string t_name=u_obj["name"],t_pass=u_obj["password"];
        double t_balance=u_obj["balance"];
        uint64_t uid=u_obj["uid"];
        user_count ++;
        using namespace nlohmann;
        json cart;
        cartObj cart_obj;
        tu=new Customer(uid,t_name,t_pass,t_balance);
        cart=u_obj["cart"];
        for(auto item:cart) {
            cart_obj.pnu.first=item["goods_name"];
            cart_obj.pnu.second=item["belong"];
            cart_obj.amount=item["amount"];
            tu->addCart(cart_obj);
        }
        if(tu) {
            user_list[t_name]=tu;
            id_to_user[uid]=tu;
        }
    }
    config_file.close();
}

void writeUserConfig() {
    ofstream config_file;
    config_file.open("config.json");
    if(!config_file.is_open()) {
        qWarning()<<"打开用户配置文件失败";
        return;
    }
    config_file<<user_json_list;
    config_file.close();
}
