#ifndef HOSTWINDOW_H
#define HOSTWINDOW_H

#include <QDialog>
#include <QTcpServer>

namespace Ui {
class HostWindow;
}

class HostWindow : public QDialog
{
    Q_OBJECT

public:
    explicit HostWindow(QWidget *parent = nullptr);
    ~HostWindow();

private:
    Ui::HostWindow *ui;

signals:
    void HostButtonClicked();
private slots:
    void on_okButton_clicked();
};

#endif // HOSTWINDOW_H
