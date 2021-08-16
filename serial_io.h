#ifndef SERIALIO_H
#define SERIALIO_H

#include <QThread>

class QSerialPort;

class SerialIO : public QThread
{
    Q_OBJECT
public:
    SerialIO(QString port_name, int baudrate, QObject* parent=nullptr);

signals:
    void imageDataParsed(const QString& name, const QImage& image);

    void imageParsed(const QString& name);

    void paramParsed(const QString& name, double min_value, double max_value, double step_value, double current_value);

    void optionParsed(const QString& name, bool current_option);

    void buttonParsed(const QString& name);

public slots:
    void sendImage(const QString& name);

    void sendParam(const QString& name, double value);

    void sendOption(const QString& name, bool option);

    void sendButton(const QString& name);

private slots:
    void tryReadPort();

private:
    void run();

    void parseOnce();

    QByteArray buffer;
    QSerialPort* port;
};

#endif // SERIALIO_H
