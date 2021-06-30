#ifndef RECHARGE_H
#define RECHARGE_H

#include <QWidget>
#include "client.h"

namespace Ui {
class recharge_w;
}

class recharge_w : public QWidget
{
    Q_OBJECT

public:
    explicit recharge_w(QWidget *parent = nullptr);
    ~recharge_w();
signals:
    void sendOk();
private slots:
    void on_rechargeButton_clicked();

    void checkCallback(bool s);

    void closeWindow();

private:
    Ui::recharge_w *ui;
};

#endif // RECHARGE_H
