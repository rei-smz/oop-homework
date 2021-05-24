#ifndef ADDGOODS_H
#define ADDGOODS_H

#include <QWidget>

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
    void sendOk();

private slots:
    void on_pushButton_clicked();

    void closeWindow();

private:
    Ui::addGoods *ui;
};

#endif // ADDGOODS_H
