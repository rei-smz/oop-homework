#include "management.h"
#include "file_io.h"

using namespace std;

unordered_map<goods_type_t,double> discount;

Food::Food(double price, const string& name, const string &desc, uint64_t number,uint64_t belong) {
    goods_type=food;
    origin_price=price;
    goods_name=name;
    description=desc;
    remain=number;
    belong_to=belong;
}

double Food::getPrice() {
    double off=id_to_user[belong_to]->getDiscount(food);
    return origin_price*(1-off);
}

Cloths::Cloths(double price, const std::string& name, const string &desc, uint64_t number,uint64_t belong) {
    goods_type=cloths;
    origin_price=price;
    goods_name=name;
    description=desc;
    remain=number;
    belong_to=belong;
}

double Cloths::getPrice() {
    double off=id_to_user[belong_to]->getDiscount(cloths);
    return origin_price*(1-off);
}

Book::Book(double price, const std::string& name, const string &desc, uint64_t number,uint64_t belong) {
    goods_type=book;
    origin_price=price;
    goods_name=name;
    description=desc;
    remain=number;
    belong_to=belong;
}

double Book::getPrice() {
    double off=id_to_user[belong_to]->getDiscount(book);
    return origin_price*(1-off);
}

Electronic::Electronic(double price, const std::string& name, const string &desc, uint64_t number,uint64_t belong) {
    goods_type=electronic;
    origin_price=price;
    goods_name=name;
    description=desc;
    remain=number;
    belong_to=belong;
}

double Electronic::getPrice() {
    double off=id_to_user[belong_to]->getDiscount(electronic);
    return origin_price*(1-off);
}

user_type_t Merchant::getUserType() {
    return user_type;
}

Customer::Customer(uint64_t id,const std::string &user, const string &pass,double balance) {
    user_id = id;
    user_type=customer;
    this->balance=balance;
    username=user;
    password=pass;
}

user_type_t Customer::getUserType() {
    return user_type;
}

void Customer::addGoods(goods_type_t type, double price, const string &name, const string &desc, int64_t number) {
    cout<<"用户不为商家，无法操作"<<endl;
}

void Customer::manageGoods(const string &name, double price, int64_t remain) {
    cout<<"用户不为商家，无法操作"<<endl;
}

void Customer::manageDiscount(goods_type_t type,double off) {
    cout<<"用户不为商家，无法操作"<<endl;
}

void Customer::deleteGoods(const string &name) {
    cout<<"用户不为商家，无法操作"<<endl;
}

void Customer::addCart(cartObj item) {
    bool flag=false;
    for(auto& i:cart) {
        if(i.pnu==item.pnu) {
            flag=true;
            i.amount+=item.amount;
            break;
        }
    }
    if(!flag) {
        cart.push_back(item);
    }
}

void Customer::delCart(cartObj item) {
    for(auto i=cart.begin(); i!=cart.end();) {
        if(item.pnu == i->pnu && item.amount == i->amount) {
            i = cart.erase(i);
        } else {
            ++i;
        }
    }
}

void Customer::changeCart(cartObj item) {
    for(auto& i:cart) {
        if(i.pnu == item.pnu) {
            i.amount = item.amount;
        }
    }
}

void Customer::recharge(double money) {
    balance+=money;
    for(auto& u:user_json_list["Customer"]) {
        if(u["uid"]==user_id) {
            double t=u["balance"];
            u["balance"]=t+money;
            break;
        }
    }
}

void Merchant::recharge(double money) {
    balance+=money;
    for(auto& u:user_json_list["Merchant"]) {
        if(u["uid"]==user_id) {
            double t=u["balance"];
            u["balance"]=t+money;
            break;
        }
    }
}

void User::changePassword(const string& old_pass,const string& new_pass) {
    if(old_pass==password) {
        password=new_pass;
    } else {
        cout<<"旧密码输入错误"<<endl;
    }
}

void Customer::pay(double money) {
    if(balance>=money) {
        balance-=money;
        for(auto& u:user_json_list["Customer"]) {
            if(u["uid"]==user_id) {
                double t=u["balance"];
                u["balance"]=t-money;
            }
        }
    } else {
        cout<<"余额不足"<<endl;
    }
}

void Merchant::pay(double money) {
    if(balance>=money) {
        balance-=money;
        for(auto& u:user_json_list["Merchant"]) {
            if(u["uid"]==user_id) {
                double t=u["balance"];
                u["balance"]=t-money;
            }
        }
    } else {
        cout<<"余额不足"<<endl;
    }
}

double User::getBalance() {
    return balance;
}

Merchant::Merchant(uint64_t id,const string& user,const string& pass,double balance) {
    user_type=merchant;
    user_id = id;
    this->balance=balance;
    username=user;
    password=pass;
    discount[food]=0;
    discount[cloths]=0;
    discount[book]=0;
    discount[electronic]=0;
}

void Merchant::addGoods(goods_type_t type,double price,const string& name,const string& desc,int64_t number) {
    Goods* t;
    switch (type) {
        case food:
            t=new Food(price,name,desc,number,user_id);
            goods_list[pair(name,user_id)]=t;
            break;
        case cloths:
            t=new Cloths(price,name,desc,number,user_id);
            goods_list[pair(name,user_id)]=t;
            break;
        case book:
            t=new Book(price,name,desc,number,user_id);
            goods_list[pair(name,user_id)]=t;
            break;
        case electronic:
            t=new Electronic(price,name,desc,number,user_id);
            goods_list[pair(name,user_id)]=t;
            break;
    }
    goods_name.push_back(std::pair(name,user_id));
}

void Merchant::manageGoods(const std::string& name,double price,int64_t remain) {
    if(price==-1&&remain==-1) {
        cout<<"修改的价格与剩余量均无效"<<endl;
        return;
    }
    auto item=goods_list[pair(name,user_id)];
    if(item!=nullptr) {
        if(price!=-1) {
            item->changeOriginPrice(price);
        }
        if(remain!=-1) {
            item->changeRemain(remain);
        }
        cout<<"修改完成"<<endl;
        return;
    }
    cout<<"需要修改的商品未找到"<<endl;
}

void Merchant::manageDiscount(goods_type_t type,double off) {
    discount[type]=off;
}

void Merchant::deleteGoods(const std::string& name) {
    auto item=goods_list[pair(name,user_id)];
    if(item!=nullptr) {
        goods_list.erase(pair(name,user_id));
        cout<<"删除成功"<<endl;
        return;
    }
    cout<<"需要删除的商品未找到"<<endl;
}

void changeUserToJson(User* u) {
    using namespace nlohmann;
    json user_object;
    user_object["name"]=u->getUserName();
    user_object["password"]=u->getUserPass();
    user_object["balance"]=u->getBalance();
    user_object["uid"]=u->getUserId();
    if(u->getUserType()==customer) {
        std::vector<cartObj> user_cart = u->getCart();
        user_object["cart"]=nlohmann::json::array();
        for(auto item:user_cart) {
            json cart_object;
            cart_object["goods_name"]=item.pnu.first;
            cart_object["belong"]=item.pnu.second;
            cart_object["amount"]=item.amount;
            user_object["cart"].push_back(cart_object);
        }
        user_json_list["Customer"].push_back(user_object);
    } else {
        user_object["discount"]={
            {"food",u->getDiscount(food)},
            {"cloths",u->getDiscount(cloths)},
            {"book",u->getDiscount(book)},
            {"electronic",u->getDiscount(electronic)}
        };
        user_json_list["Merchant"].push_back(user_object);
    }

}

void refreshUserJson(User* u) {
    using namespace nlohmann;
    user_type_t type = u->getUserType();
    if(type == merchant) {
        uint64_t index;
        for(index = 0; index < user_json_list["Merchant"].size(); index++) {
            if(user_json_list["Merchant"].at(index)["uid"] == u->getUserId()) {
                break;
            }
        }
        user_json_list["Merchant"].at(index)["discount"] = {
            {"food",u->getDiscount(food)},
            {"cloths",u->getDiscount(cloths)},
            {"book",u->getDiscount(book)},
            {"electronic",u->getDiscount(electronic)}
        };
        std::cout<<user_json_list["Merchant"]<<std::endl;
    } else {
        uint64_t index;
        for(index = 0; index < user_json_list["Customer"].size(); index++) {
            if(user_json_list["Customer"].at(index)["uid"] == u->getUserId()) {
                break;
            }
        }
        std::vector<cartObj> user_cart = u->getCart();
        user_json_list["Customer"].at(index)["cart"]=nlohmann::json::array();
        for(auto item:user_cart) {
            json cart_object;
            cart_object["goods_name"]=item.pnu.first;
            cart_object["belong"]=item.pnu.second;
            cart_object["amount"]=item.amount;
            user_json_list["Customer"].at(index)["cart"].push_back(cart_object);
        }
    }
    writeUserConfig();
}
