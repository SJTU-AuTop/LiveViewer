#include "liveviewer.h"
#include "ui_liveviewer.h"

#include "portselectdialog.h"
#include "serial_io.h"
#include "viewerparam.h"
#include "vieweroption.h"
#include "viewerbutton.h"

#include <QTime>
#include <QSerialPort>
#include <QSerialPortInfo>

LiveViewer::LiveViewer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LiveViewer)
    , io(nullptr)
{
    ui->setupUi(this);

// FOR DEBUG TEST
//    ui->paramScrollAreaWidgetContents->layout()->addWidget(new ViewerParam("TEST", 0.1, 5.2, 0.12, 1));
//    ui->optionScrollAreaWidgetContents->layout()->addWidget(new ViewerOption("TEST", QStringList{"A", "B"}, "C"));
//    ui->buttonScrollAreaWidgetContents->layout()->addWidget(new ViewerButton("TEST"));
}

LiveViewer::~LiveViewer()
{
    delete ui;
    ui = nullptr;
}

void LiveViewer::reset_ui(){
    ui->imageComboBox->clear();
    ui->displayLabel->clear();
    ui->fpsLabel->setText("fps=0");
    QLayoutItem *child = nullptr;
    while(child = ui->paramScrollAreaWidgetContents->layout()->takeAt(0)){
        delete child->widget();
        delete child;
    }
    while(child = ui->optionScrollAreaWidgetContents->layout()->takeAt(0)){
        delete child->widget();
        delete child;
    }
    while(child = ui->buttonScrollAreaWidgetContents->layout()->takeAt(0)){
        delete child->widget();
        delete child;
    }
    if(io != nullptr){
        io->deleteLater();
        io = nullptr;
    }
}

void LiveViewer::updateImage(const QString &name){
    if(ui->imageComboBox->findText(name) == -1){
        ui->imageComboBox->addItem(name);
    }
}

void LiveViewer::updateImageData(const QString &name, const QImage& image){
    int idx = ui->imageComboBox->findText(name);
    if(idx == -1){
        ui->imageComboBox->addItem(name);
        ui->imageComboBox->setCurrentText(name);
    }else{
        ui->imageComboBox->setCurrentIndex(idx);
    }
    ui->displayLabel->setPixmap(QPixmap::fromImage(image.scaled(ui->displayLabel->size(), Qt::KeepAspectRatio)));
    // update fps
    static int fps = 0;
    static int t1 = QTime::currentTime().second();
    int t2 = QTime::currentTime().second();
    fps++;
    if(t2 > t1){
        ui->fpsLabel->setText(QString("fps=%1").arg(fps));
        fps = 0;
        t1 = t2;
    }
}

void LiveViewer::updateParam(const QString &name, double min_value, double max_value, double step_value, double current_value){
    qDebug("[%s]: %d", __FUNCTION__, QThread::currentThread());
    bool found = false;
    for(int i=0; i<ui->paramScrollAreaWidgetContents->layout()->count(); i++){
        auto *obj = ui->paramScrollAreaWidgetContents->layout()->itemAt(i)->widget();
        if(obj->objectName() == name){
            found = true;
            static_cast<ViewerParam*>(obj)->setValue(current_value);
            break;
        }
    }
    if(!found){
        auto *param = new ViewerParam(name, min_value, max_value, step_value, current_value);
        if(io != nullptr) QObject::connect(param, &ViewerParam::valueChanged, io, &SerialIO::sendParam);
        ui->paramScrollAreaWidgetContents->layout()->addWidget(param);
    }
}

void LiveViewer::updateOption(const QString &name, bool current_option){
    qDebug("[%s]: %d", __FUNCTION__, QThread::currentThread());
    bool found = false;
    for(int i=0; i<ui->optionScrollAreaWidgetContents->layout()->count(); i++){
        auto *obj = ui->optionScrollAreaWidgetContents->layout()->itemAt(i)->widget();
        if(obj->objectName() == name){
            found = true;
            static_cast<ViewerOption*>(obj)->setChecked(current_option);
            break;
        }
    }
    if(!found){
        auto *option = new ViewerOption(name, current_option);
        if(io != nullptr) QObject::connect(option, &ViewerOption::stateChanged, io, &SerialIO::sendOption);
        ui->optionScrollAreaWidgetContents->layout()->addWidget(option);
    }
}

void LiveViewer::updateButton(const QString &name){
    qDebug("[%s]: %d", __FUNCTION__, QThread::currentThread());
    bool found = false;
    for(int i=0; i<ui->buttonScrollAreaWidgetContents->layout()->count(); i++){
        auto *obj = ui->buttonScrollAreaWidgetContents->layout()->itemAt(i)->widget();
        if(obj->objectName() == name){
            found = true;
            break;
        }
    }
    if(!found){
        auto *button = new ViewerButton(name);
        if(io != nullptr) QObject::connect(button, &ViewerButton::clicked, io, &SerialIO::sendButton);
        ui->buttonScrollAreaWidgetContents->layout()->addWidget(button);
    }
}

void LiveViewer::on_menuSelectCOM_aboutToShow(){
    ui->menuSelectCOM->clear();
    for(auto &port:QSerialPortInfo::availablePorts()){
        ui->menuSelectCOM->addAction(QString("%1(%2)").arg(port.portName(), port.description()));
    }
    if(ui->menuSelectCOM->isEmpty()){
        ui->menuSelectCOM->addAction("no available port")->setDisabled(true);
    }
}

void LiveViewer::on_menuSelectCOM_triggered(QAction* obj){
    PortSelectDialog dialog(obj->text());
    if(dialog.exec() == QDialog::Accepted){
        // stop running io thread
        if(io != nullptr) io->quit();
        if(io != nullptr) io->wait();
        if(io != nullptr) QObject::disconnect(io, &QThread::finished, this, &LiveViewer::reset_ui);
        reset_ui();
        // open serial port
        QString port_name = obj->text().split("(")[0];
        // define io thread
        io = new SerialIO(port_name, dialog.baudrate());
        // connect signals
        QObject::connect(io, &SerialIO::imageParsed, this, &LiveViewer::updateImage);
        QObject::connect(io, &SerialIO::imageDataParsed, this, &LiveViewer::updateImageData);
        QObject::connect(io, &SerialIO::paramParsed, this, &LiveViewer::updateParam);
        QObject::connect(io, &SerialIO::optionParsed, this, &LiveViewer::updateOption);
        QObject::connect(io, &SerialIO::buttonParsed, this, &LiveViewer::updateButton);
        QObject::connect(io, &QThread::finished, this, &LiveViewer::reset_ui);
        QObject::connect(ui->imageComboBox, &QComboBox::currentTextChanged, io, &SerialIO::sendImage);
        // start io thread
        io->start();
    }
}
