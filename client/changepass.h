#ifndef CHANGEPASS_H
#define CHANGEPASS_H

#include <QWidget>
#include "client.h"

namespace Ui {
class changepass;
}

class changepass : public QWidget
{
    Q_OBJECT

public:
    explicit changepass(QWidget *parent = nullptr);
    ~changepass();

private slots:
    void on_pushButton_2_clicked();

    void closeWindow();

    void checkCallback(const QString err);

private:
    Ui::changepass *ui;
};

#endif // CHANGEPASS_H
