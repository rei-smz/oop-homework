#ifndef MANAGEGOODS_H
#define MANAGEGOODS_H

#include <QWidget>

namespace Ui {
class manageGoods;
}

class manageGoods : public QWidget
{
    Q_OBJECT

public:
    explicit manageGoods(QWidget *parent = nullptr);
    ~manageGoods();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void closeWindow();

signals:
    void sendOk();

private:
    Ui::manageGoods *ui;
};

#endif // MANAGEGOODS_H
