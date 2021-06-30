#ifndef BILL_H
#define BILL_H

#include <QDialog>

namespace Ui {
class bill;
}

class bill : public QDialog
{
    Q_OBJECT

public:
    explicit bill(QWidget *parent = nullptr);
    ~bill();

private:
    Ui::bill *ui;
};

#endif // BILL_H
