#include "bill.h"
#include "ui_bill.h"
#include <QPushButton>

bill::bill(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bill)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("支付");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setAutoDefault(false);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setDefault(false);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setAutoDefault(true);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setDefault(true);
}

bill::~bill()
{
    delete ui;
}
