#ifndef CARTWINDOW_H
#define CARTWINDOW_H

#include <QWidget>

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

private:
    Ui::cartWindow *ui;
};

#endif // CARTWINDOW_H
