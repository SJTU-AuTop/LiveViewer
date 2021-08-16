#include "viewerparam.h"
#include "ui_viewerparam.h"

ViewerParam::ViewerParam(QString name, double min_value, double max_value, double step_value, double current_value, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewerParam)
{
    ui->setupUi(this);
    // initialize
    setObjectName(name);
    ui->label->setText(name);
    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum((max_value-min_value) / step_value);
    ui->doubleSpinBox->setMinimum(min_value);
    ui->doubleSpinBox->setMaximum(max_value);
    ui->doubleSpinBox->setSingleStep(step_value);
    // inner connect
    QObject::connect(ui->horizontalSlider, &QSlider::valueChanged, this,
                     [this](int value){ui->doubleSpinBox->setValue(value*ui->doubleSpinBox->singleStep()+ui->doubleSpinBox->minimum());});
    QObject::connect(ui->doubleSpinBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this,
                     [this](double value){ui->horizontalSlider->setValue((value-ui->doubleSpinBox->minimum())/ui->doubleSpinBox->singleStep());});
    // outer connect
    QObject::connect(ui->doubleSpinBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this,
                     [this](double value){emit valueChanged(objectName(), value);});
    // set value
    ui->doubleSpinBox->setValue(current_value);
}

ViewerParam::~ViewerParam()
{
    delete ui;
}

void ViewerParam::setValue(double value)
{
    ui->doubleSpinBox->setValue(value);
}

double ViewerParam::value() const
{
    return ui->doubleSpinBox->value();
}
