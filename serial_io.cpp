#include "serial_io.h"
#include "base64.h"
#include <QSerialPort>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QImage>

SerialIO::SerialIO(QString port_name, int baudrate, QObject* parent) : QThread(parent), port(new QSerialPort(port_name))
{
    // initialize
    buffer.reserve(4*(1<<20)); // 4MB
    port->setBaudRate(baudrate);
    port->setReadBufferSize(4*(1<<20));
    port->moveToThread(this);
    moveToThread(this);
    // inner connect
    QObject::connect(port, &QSerialPort::readyRead, this, &SerialIO::tryReadPort);
    QObject::connect(port, &QSerialPort::errorOccurred, this, [this](auto error){
        if(error != QSerialPort::SerialPortError::NoError){
            qDebug("port error=%d", error);
            quit();
        }
    });
}

void SerialIO::run(){
    if(!port->open(QIODevice::ReadWrite)){
        quit();
    }else{
        // start event loop
        exec();
    }
}

void SerialIO::sendImage(const QString& name){
    if(port->write(qPrintable(QString("image %1").arg(name))) == -1){
        quit();
    }
}

void SerialIO::sendParam(const QString& name, double value){
    if(port->write(qPrintable(QString("param %1 %2").arg(name).arg(value))) == -1){
        quit();
    }
}

void SerialIO::sendOption(const QString& name, bool option){
    if(port->write(qPrintable(QString("option %1 %2").arg(name).arg((int)option))) == -1){
        quit();
    }
}

void SerialIO::sendButton(const QString& name){
    if(port->write(qPrintable(QString("button %1").arg(name))) == -1){
        quit();
    }
}

void SerialIO::tryReadPort(){
    auto data = port->readAll();
    for(auto ch: data){
        if(ch == '\n'){
            parseOnce();
            buffer.clear();
        }else{
            buffer.append(ch);
        }
    }
}

#define PARSE_ASSERT(expr) do{if(!(expr)){qDebug("'"#expr"' assert fail!"); return;}}while(0)
#define PARSE_CHECK(obj, name, type) do{PARSE_ASSERT(obj.contains(name));PARSE_ASSERT(obj[name].is##type());}while(0);

void SerialIO::parseOnce(){
    auto doc = QJsonDocument::fromJson(buffer);
    PARSE_ASSERT(!doc.isEmpty());
    auto obj = doc.object();
    PARSE_CHECK(obj, "type", String);
    PARSE_CHECK(obj, "body", Object);
    auto type = obj["type"].toString();
    auto body = obj["body"].toObject();
    if(type == "image"){
        PARSE_CHECK(body, "name", String);
        if(body.contains("width") && body.contains("height") && body.contains("data")){
            PARSE_ASSERT(body["width"].isDouble());
            PARSE_ASSERT(body["height"].isDouble());
            PARSE_ASSERT(body["data"].isString());
            int width = body["width"].toInt();
            int height = body["height"].toInt();
            QByteArray base64_data = body["data"].toString().toUtf8();
            QImage image(width, height, QImage::Format_Grayscale8);
            from_base64(image.bits(), (uint8_t*)base64_data.data(), base64_data.length());
            emit imageDataParsed(body["name"].toString(), image);
        }else{
            emit imageParsed(body["name"].toString());
        }
    }else if(type == "param"){
        PARSE_CHECK(body, "name", String);
        PARSE_CHECK(body, "min_value", Double);
        PARSE_CHECK(body, "max_value", Double);
        PARSE_CHECK(body, "step_value", Double);
        PARSE_CHECK(body, "current_value", Double);
        emit paramParsed(body["name"].toString(), body["min_value"].toDouble(), body["max_value"].toDouble(),
                         body["step_value"].toDouble(), body["current_value"].toDouble());
    }else if(type == "option"){
        PARSE_CHECK(body, "name", String);
        PARSE_CHECK(body, "current_option", Bool);
        emit optionParsed(body["name"].toString(), body["current_option"].toBool());
    }else if(type == "button"){
        PARSE_CHECK(body, "name", String);
        emit buttonParsed(body["name"].toString());
    }else{
        qDebug("unknown type '%s'", qPrintable(type));
    }
}
