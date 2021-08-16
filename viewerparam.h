#ifndef VIEWERPARAM_H
#define VIEWERPARAM_H

#include <QWidget>

namespace Ui {
class ViewerParam;
}

class ViewerParam : public QWidget
{
    Q_OBJECT

public:
    explicit ViewerParam(QString name, double min_value, double max_value, double step_value, double current_value, QWidget *parent = nullptr);
    ~ViewerParam();

signals:
    void valueChanged(QString obj_name, double value);

public slots:
    void setValue(double value);

public:
    double value() const;

private:
    Ui::ViewerParam *ui;
};

#endif // VIEWERPARAM_H
