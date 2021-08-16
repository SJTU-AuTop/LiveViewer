#ifndef VIEWEROPTION_H
#define VIEWEROPTION_H

#include <QWidget>

namespace Ui {
class ViewerOption;
}

class ViewerOption : public QWidget
{
    Q_OBJECT

public:
    explicit ViewerOption(QString name, bool current_option, QWidget *parent = nullptr);
    ~ViewerOption();

signals:
    void stateChanged(QString obj_name, bool current_option);

public slots:
    void setChecked(bool current_option);

private:
    Ui::ViewerOption *ui;
};

#endif // VIEWEROPTION_H
