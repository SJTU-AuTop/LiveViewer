#ifndef VIEWERBUTTON_H
#define VIEWERBUTTON_H

#include <QWidget>

namespace Ui {
class ViewerButton;
}

class ViewerButton : public QWidget
{
    Q_OBJECT

public:
    explicit ViewerButton(QString name, QWidget *parent = nullptr);
    ~ViewerButton();

signals:
    void clicked(const QString& name);

private:
    Ui::ViewerButton *ui;
};

#endif // VIEWERBUTTON_H
