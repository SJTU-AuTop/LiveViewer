#include "portselectdialog.h"
#include "ui_portselectdialog.h"
#include <QValidator>

PortSelectDialog::PortSelectDialog(QString port, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PortSelectDialog)
{
    ui->setupUi(this);
    // initialize
    ui->portLabel->setText(port);
    ui->lineEdit->setValidator(new QRegExpValidator(QRegExp("^[0-9]*[1-9][0-9]*$")));
}

PortSelectDialog::~PortSelectDialog()
{
    delete ui;
}

int PortSelectDialog::baudrate() const{
    return ui->lineEdit->text().toInt();
}
