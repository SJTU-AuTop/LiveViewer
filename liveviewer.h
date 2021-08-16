#ifndef LIVEVIEWER_H
#define LIVEVIEWER_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class LiveViewer; }
QT_END_NAMESPACE

class SerialIO;

class LiveViewer : public QMainWindow
{
    Q_OBJECT

public:
    LiveViewer(QWidget *parent = nullptr);
    ~LiveViewer();

public slots:
    void reset_ui();

    void updateImage(const QString &name);

    void updateImageData(const QString &name, const QImage& image);

    void updateParam(const QString &name, double min_value, double max_value, double step_value, double current_value);

    void updateOption(const QString &name, bool current_option);

    void updateButton(const QString &name);

private slots:
    void on_menuSelectCOM_aboutToShow();

    void on_menuSelectCOM_triggered(QAction*);

private:
    Ui::LiveViewer *ui;
    SerialIO *io;
};
#endif // LIVEVIEWER_H
