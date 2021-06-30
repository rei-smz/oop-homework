#ifndef ADDGOODS_H
#define ADDGOODS_H

#include <QWidget>
#include "client.h"

namespace Ui {
class addGoods;
}

class addGoods : public QWidget
{
    Q_OBJECT

public:
    explicit addGoods(QWidget *parent = nullptr);
    ~addGoods();

signals:

private slots:
    void on_pushButton_clicked();

    void closeWindow();

    void checkCallback(bool s);

private:
    Ui::addGoods *ui;
};

#endif // ADDGOODS_H
