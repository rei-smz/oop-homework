#ifndef CARTWINDOW_H
#define CARTWINDOW_H

#include <QWidget>
#include "client.h"

namespace Ui {
class cartWindow;
}

class cartWindow : public QWidget
{
    Q_OBJECT

public:
    explicit cartWindow(QWidget *parent = nullptr);
    ~cartWindow();

signals:
    void sendPayOk();

private slots:
    void refresh_cart();

    void on_delButton_clicked();

    void on_purchaseButton_clicked();

    void closeWindow();

    void on_changeButton_clicked();

    void checkCart(bool s);

    void checkDelCallback(bool s);

    void checkPurchaseCallback(const QString err);

    void checkChangeCallback(bool s);

    void onReCart(const QString data);

private:
    Ui::cartWindow *ui;
};

#endif // CARTWINDOW_H
