#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QWidget>
#include "client.h"

namespace Ui {
class account;
}

class account : public QWidget
{
    Q_OBJECT

public:
    explicit account(QWidget *parent = nullptr);
    ~account();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void checkLoginCallback(const QString err);

    void checkRegCallback(const QString err);

signals:
    void sendOk();
private:
    Ui::account *ui;
};

#endif // ACCOUNT_H
