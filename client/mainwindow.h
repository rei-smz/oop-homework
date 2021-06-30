#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void sendAddCartOk(bool s);
    void sendAccountChanged();
    void sendLoginStatus(const QString err);
    void sendRegOk(const QString err);
    void sendAddGoodsOk(bool s);
    void sendChangePassOk(const QString err);
    void sendDelGoodsOk(bool s);
    void sendChgRemainOk(bool s);
    void sendChgOPriceOk(bool s);
    void sendChgDiscountOk(bool s);
    void sendRechargeOk(bool s);
    void sendDelCartOk(bool s);
    void sendPurchaseOk(const QString err);
    void sendChangeCartOk(bool s);
    void sendReCartData(const QString data);


private slots:
    void on_dengluzhuce_triggered();

    void on_xiugaimima_triggered();

    void on_actiontuichu_triggered();

    void refresh_goods();

//    void append_goods();

    void refresh_user();

    void on_actionrecharge_triggered();

    void on_actionaddGoods_triggered();

    void on_actionmanage_triggered();

    void on_pushButton_clicked();

    void on_addCartButton_clicked();

    void on_actioncart_triggered();

    void onReadyRead();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
