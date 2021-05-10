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

extern std::unordered_map<goods_type_t,double> discount;


class Goods {
protected:
    goods_type_t goods_type;
    double origin_price;
    std::string goods_name;
    std::string description;
    int64_t remain;
public:
    virtual double getPrice()=0;
    inline virtual goods_type_t getType() {
        return goods_type;
    }
    inline virtual std::string getName() {
        return goods_name;
    }
    inline virtual std::string getDesc() {
        return description;
    }
    inline virtual int64_t getRemain() {
        return remain;
    }
    inline virtual void changeOriginPrice(double new_price) {
        origin_price=new_price;
    }
    inline virtual void changeRemain(int64_t new_remain) {
        remain=new_remain;
    }
};

class Food: public Goods {
public:
    Food(double price,const std::string& name,const std::string& desc,int64_t number);
    double getPrice() override;
};

class Cloths: public Goods {
public:
    Cloths(double price,const std::string& name,const std::string& desc,int64_t number);
    double getPrice() override;
};

class Book: public Goods {
public:
    Book(double price,const std::string& name,const std::string& desc,int64_t number);
    double getPrice() override;
};

class Electronic: public Goods {
public:
    Electronic(double price,const std::string& name,const std::string& desc,int64_t number);
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
    virtual void recharge(double money);
    virtual double getBalance();
    virtual void pay(double money);
    virtual void changePassword(const std::string& old_pass,const std::string& new_pass);
    inline virtual std::string getUserName() {
        return username;
    }
    inline virtual std::string getUserPass() {
        return password;
    }
    inline virtual uint64_t getUserId() {
        return user_id;
    }
};

class Merchant: public User {
public:
    Merchant(uint64_t id,const std::string& user,const std::string& pass,double balance);
    user_type_t getUserType() override;
    void addGoods(goods_type_t type,double price,const std::string& name,const std::string& desc,int64_t number) final;
    void manageGoods(const std::string& name,double price,int64_t remain) final;
    void manageDiscount(goods_type_t type,double off) final;
    void deleteGoods(const std::string& name) final;
};

class Customer: public User {
public:
    Customer(uint64_t id,const std::string& user,const std::string& pass,double balance);
    user_type_t getUserType() override;
    void addGoods(goods_type_t type,double price,const std::string& name,const std::string& desc,int64_t number) final;
    void manageGoods(const std::string& name,double price,int64_t remain) final;
    void manageDiscount(goods_type_t type,double off) final;
    void deleteGoods(const std::string& name) final;
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
#endif
