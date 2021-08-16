#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QString>
#include <QList>

struct ViewerImage{
    int width;
    int height;
    QByteArray data;
};

struct ViewerParam{
    QString name;
    double min_value;
    double max_value;
    double current_value;
};

struct ViewerOption{
    QString name;
    QStringList options;
    QString current_option;
};

struct ViewerButton{
    QString name;
};

#endif // PROTOCOL_H
