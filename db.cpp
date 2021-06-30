#include "db.h"
#include "management.h"

void insertGoods() {
    while(id<goods_name.size()) {
        std::pair<std::string,uint64_t> pnu = goods_name[id];
        Goods* g=goods_list[pnu];
        std::string g_desc=g->getDesc();
        int g_type=g->getType();
        double g_price=g->getOriginPrice();
        int64_t g_remain=g->getRemain();
        QString cmd=QString("insert into goods values(%1,'").arg(id)+QString::fromStdString(pnu.first)+QString("','")+QString::fromStdString(g_desc)+QString("',%1,%2,%3,%4)").arg(g_type).arg(g_price).arg(g_remain).arg(pnu.second);
        query.exec(cmd);
        id++;
    }
}

void changeGoods(const std::string& g_name,uint64_t uid) {
    query.exec(QString("update goods set origin_price = %1 where name = '").arg(goods_list[std::pair(g_name,uid)]->getPrice())+QString::fromStdString(g_name)+QString("' and belong_to = %1").arg(uid));
    query.exec(QString("update goods set remain = %1 where name = '").arg(goods_list[std::pair(g_name,uid)]->getRemain())+QString::fromStdString(g_name)+QString("' and belong_to = %1").arg(uid));
}

void deleteGoods(const std::string& g_name,uint64_t uid) {
    int64_t index=0;
    for(auto i=goods_name.begin();i!=goods_name.end();) {
        if(*i==std::pair(g_name,uid)) {
            index=i-goods_name.begin();
            i=goods_name.erase(i);
        } else {
            i++;
        }
    }
    query.exec(QString("delete from goods where name = '")+QString::fromStdString(g_name)+QString("' and belong_to = %1").arg(uid));
    query.exec(QString("update goods set id = id - 1 where id > %1").arg(index));
}

void loadGoods() {
    query.exec(QString("select * from goods where id < %1").arg(id));
    while (query.next()) {
        goods_count++;
        Goods* g;
        std::string g_name=query.value(1).toString().toStdString();
        std::string g_desc=query.value(2).toString().toStdString();
        int g_type=query.value(3).toInt();
        double g_price=query.value(4).toDouble();
        int64_t g_remain=query.value(5).toLongLong();
        uint64_t g_belong=query.value(6).toULongLong();
        goods_name.push_back(std::pair(g_name,g_belong));
        std::pair<std::string,uint64_t> pnu(g_name,g_belong);
        switch (g_type) {
            case 0:
                g=new Food(g_price,g_name,g_desc,g_remain,g_belong);
                goods_list[pnu]=g;
                break;
            case 1:
                g=new Cloths(g_price,g_name,g_desc,g_remain,g_belong);
                goods_list[pnu]=g;
                break;
            case 2:
                g=new Book(g_price,g_name,g_desc,g_remain,g_belong);
                goods_list[pnu]=g;
                break;
            case 3:
                g=new Electronic(g_price,g_name,g_desc,g_remain,g_belong);
                goods_list[pnu]=g;
                break;
            default:
                break;
        }
    }
    qDebug()<<"读取商品数据完成";
}
