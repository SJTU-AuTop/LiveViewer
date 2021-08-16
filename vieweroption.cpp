#include "vieweroption.h"
#include "ui_vieweroption.h"

ViewerOption::ViewerOption(QString name, bool current_option, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewerOption)
{
    ui->setupUi(this);
    // initialize
    setObjectName(name);
    ui->checkBox->setText(name);
    ui->checkBox->setChecked(current_option);
    // outer connect
    QObject::connect(ui->checkBox, &QCheckBox::stateChanged, this,
                     [this](int state){ emit stateChanged(objectName(), state!=Qt::Unchecked); });
}

ViewerOption::~ViewerOption()
{
    delete ui;
}

void ViewerOption::setChecked(bool current_option){
    ui->checkBox->setChecked(current_option);
}
