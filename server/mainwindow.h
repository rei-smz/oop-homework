#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define CMD_TEST 0
#define CMD_LOGIN 1
#define CMD_LOGOUT 13
#define CMD_REG 2
#define CMD_CHG_PASS 3
#define CMD_ADD_GOODS 4
#define CMD_DEL_GOODS 5
#define CMD_CHG_REMAIN 6
#define CMD_CHG_OPRICE 7
#define CMD_CHG_DISCOUNT 8
#define CMD_ADD_CART 9
#define CMD_DEL_CART 10
#define CMD_CHG_CART 15
#define CMD_PURCHASE 11
#define CMD_RE_CART 12
#define CMD_RE_GOODS 14
#define CMD_RECHARGE 16

#define CB_TEST_OK 0
#define CB_RE_USER 1
#define CB_REG 2
#define CB_CHG_PASS 3
#define CB_ADD_GOODS 4
#define CB_DEL_GOODS 5
#define CB_CHG_REMAIN 6
#define CB_CHG_OPRICE 7
#define CB_CHG_DISCOUNT 8
#define CB_ADD_CART 9
#define CB_DEL_CART 10
#define CB_CHG_CART 14
#define CB_PURCHASE 11
#define CB_RE_CART 12
#define CB_RE_GOODS 13
#define CB_RECHARGE 15

#include <QMainWindow>
#include <nlohmann/json.hpp>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onNewConnection();

    void checkClient();

    void onReadyRead();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void get_command(const std::string msg, uint socket_id);
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
