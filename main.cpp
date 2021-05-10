#include "mainwindow.h"
#include "file_io.h"
#include "management.h"
#include "db.h"
#include <QDebug>
#include <QApplication>

nlohmann::json user_json_list=nlohmann::json::array();
std::vector<std::pair<std::string,uint64_t>> goods_name;
std::unordered_map<std::pair<std::string,uint64_t>,Goods*,hash_pair> goods_list;
std::unordered_map<std::string,User*> user_list;
std::unordered_map<uint64_t,User*> id_to_user;
User* current_user=nullptr;
int64_t id;
QSqlDatabase db;
QSqlQuery query;
std::unordered_map<std::string,goods_type_t> type_map;
uint64_t user_count;
uint64_t goods_count;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*打开数据库*/
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../../../goods.db");
    query=QSqlQuery(db);
    if(!db.open()) {
        exit(1);
    }

    user_count=0;
    goods_count=0;

    /*读取用户信息*/
    readUserConfig();

    /*检查数据表是否存在*/
    query.exec("select * from sqlite_master where type='table' and name='goods'");
    if(!query.next()) {
        query.exec("create table goods(id integer primary key autoincrement,name varchar,description varchar,type int,origin_price double,remain int,belong_to int)");
        qDebug()<<"创建数据表成功";
    }

    /*检查是否有数据*/
    query.exec("select max(id) from goods");
    if(query.next()&&query.value(0).toLongLong()!=0) { //数据表已添加数据
        id=1+query.value(0).toLongLong();
        qDebug()<<"数据表内有数据，读取中…";
        loadGoods();
    } else {
        id=0;
    }

    discount[food]=0;
    discount[cloths]=0;
    discount[book]=0;
    discount[electronic]=0;
    type_map["食品"]=food;
    type_map["服装"]=cloths;
    type_map["书籍"]=book;
    type_map["电子产品"]=electronic;

    qDebug()<<"初始化完成";
    MainWindow w;
    w.show();
    return a.exec();
}
