#include "viewerbutton.h"
#include "ui_viewerbutton.h"

ViewerButton::ViewerButton(QString name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewerButton)
{
    ui->setupUi(this);
    // initialize
    setObjectName(name);
    ui->label->setText(name);
    // outer connect
    QObject::connect(ui->pushButton, &QPushButton::clicked, this, [this](){emit clicked(objectName());});
}

ViewerButton::~ViewerButton()
{
    delete ui;
}
