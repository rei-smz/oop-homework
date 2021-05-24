//
// Created by Rin Shimizu on 13/4/21.
//
#pragma once

#ifndef MANAGEMENT_H
#define MANAGEMENT_H
#include <iostream>
#include <unordered_map>
#include <vector>
#include <utility>
#include <nlohmann/json.hpp>

enum user_type_t {merchant=0,customer=1};
enum goods_type_t {food=0,cloths=1,book=2,electronic=3} ;


typedef struct cartObj {
    std::pair<std::string,uint64_t> pnu;
    uint64_t amount;
}cartObj ;

class Goods {
protected:
    goods_type_t goods_type;
    double origin_price;
    std::string goods_name;
    std::string description;
    uint64_t remain;
    uint64_t belong_to;
public:
    virtual double getPrice()=0;
    inline goods_type_t getType() {
        return goods_type;
    }
    inline std::string getName() {
        return goods_name;
    }
    inline std::string getDesc() {
        return description;
    }
    inline uint64_t getRemain() {
        return remain;
    }
    inline void changeOriginPrice(double new_price) {
        origin_price=new_price;
    }
    inline void changeRemain(int64_t new_remain) {
        remain=new_remain;
    }
    inline uint64_t getBelong() {
        return belong_to;
    }
    inline double getOriginPrice() {
        return origin_price;
    }
};

class Food: public Goods {
public:
    Food(double price,const std::string& name,const std::string& desc,uint64_t number,uint64_t belong);
    double getPrice() override;
};

class Cloths: public Goods {
public:
    Cloths(double price,const std::string& name,const std::string& desc,uint64_t number,uint64_t belong);
    double getPrice() override;
};

class Book: public Goods {
public:
    Book(double price,const std::string& name,const std::string& desc,uint64_t number,uint64_t belong);
    double getPrice() override;
};

class Electronic: public Goods {
public:
    Electronic(double price,const std::string& name,const std::string& desc,uint64_t number,uint64_t belong);
    double getPrice() override;
};

class User {
protected:
    std::string username;
    std::string password;
    double balance;
    user_type_t user_type;
    uint64_t user_id;
public:
    virtual user_type_t getUserType()=0;
    virtual void addGoods(goods_type_t type,double price,const std::string& name,const std::string& desc,int64_t number)=0;
    virtual void manageGoods(const std::string& name,double price,int64_t remain)=0;
    virtual void manageDiscount(goods_type_t type,double off)=0;
    virtual void deleteGoods(const std::string& name)=0;
    virtual std::vector<cartObj> getCart()=0;
    virtual void addCart(cartObj item)=0;
    virtual void delCart(cartObj item)=0;
    virtual void changeCart(cartObj item)=0;
    virtual double getDiscount(goods_type_t type)=0;
    virtual void recharge(double money)=0;
    double getBalance();
    virtual void pay(double money)=0;
    void changePassword(const std::string& old_pass,const std::string& new_pass);
    inline std::string getUserName() {
        return username;
    }
    inline std::string getUserPass() {
        return password;
    }
    inline uint64_t getUserId() {
        return user_id;
    }
};

class Merchant: public User {
    std::unordered_map<goods_type_t,double> discount;
public:
    Merchant(uint64_t id,const std::string& user,const std::string& pass,double balance);
    user_type_t getUserType() override;
    void addGoods(goods_type_t type,double price,const std::string& name,const std::string& desc,int64_t number) final;
    void manageGoods(const std::string& name,double price,int64_t remain) final;
    void manageDiscount(goods_type_t type,double off) final;
    void deleteGoods(const std::string& name) final;
    void recharge(double money) final;
    void pay(double money) final;
    inline std::vector<cartObj> getCart() final {
        std::cout<<"用户不为消费者"<<std::endl;
        return {};
    }
    inline void addCart(cartObj) final {
        std::cout<<"用户不为消费者"<<std::endl;
    }
    inline void delCart(cartObj) final {
        std::cout<<"用户不为消费者"<<std::endl;
    }
    inline void changeCart(cartObj) final {
        std::cout<<"用户不为消费者"<<std::endl;
    }
    inline double getDiscount(goods_type_t type) final {
        return discount[type];
    }
};

class Customer: public User {
    std::vector<cartObj> cart;
public:
    Customer(uint64_t id,const std::string& user,const std::string& pass,double balance);
    user_type_t getUserType() override;
    void addGoods(goods_type_t type,double price,const std::string& name,const std::string& desc,int64_t number) final;
    void manageGoods(const std::string& name,double price,int64_t remain) final;
    void manageDiscount(goods_type_t type,double off) final;
    void deleteGoods(const std::string& name) final;
    inline std::vector<cartObj> getCart() final {
        return cart;
    }
    void recharge(double money) final;
    void addCart(cartObj item) final;
    void delCart(cartObj item) final;
    void changeCart(cartObj item) final;
    void pay(double money) final;
    inline double getDiscount(goods_type_t) final {
        std::cout<<"用户不为商家"<<std::endl;
        return -1;
    }
};

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const
    {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

extern nlohmann::json user_json_list;
extern std::vector<std::pair<std::string,uint64_t>> goods_name;
extern std::unordered_map<std::pair<std::string,uint64_t>,Goods*,hash_pair> goods_list;
extern std::unordered_map<std::string,User*> user_list;
extern std::unordered_map<uint64_t,User*> id_to_user;
extern User* current_user;
extern std::unordered_map<std::string,goods_type_t> type_map;
extern uint64_t goods_count,user_count;

void changeUserToJson(User* u);
void refreshUserJson(User* u);
#endif
