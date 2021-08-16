#ifndef PORTSELECTDIALOG_H
#define PORTSELECTDIALOG_H

#include <QDialog>

namespace Ui {
class PortSelectDialog;
}

class PortSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PortSelectDialog(QString port, QWidget *parent = nullptr);
    ~PortSelectDialog();

    int baudrate() const;

private:
    Ui::PortSelectDialog *ui;
};

#endif // PORTSELECTDIALOG_H
