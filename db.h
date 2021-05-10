//
// Created by Rin Shimizu on 19/4/21.
//

#ifndef ACCOUNT_AND_GOODS_CLI_DB_H
#define ACCOUNT_AND_GOODS_CLI_DB_H
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
extern QSqlDatabase db;
extern QSqlQuery query;
extern int64_t id;
void insertGoods();
void changeGoods(const std::string& g_name,uint64_t uid);
void deleteGoods(const std::string& g_name,uint64_t uid);
void findGoodsByType();
void loadGoods();
#endif //ACCOUNT_AND_GOODS_CLI_DB_H
