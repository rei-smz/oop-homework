#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_dengluzhuce_triggered();

    void on_xiugaimima_triggered();

    void on_actiontuichu_triggered();

    void refresh_goods();

    void append_goods();

    void refresh_user();

    void on_actionrecharge_triggered();

    void on_actionaddGoods_triggered();

    void on_actionmanage_triggered();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
