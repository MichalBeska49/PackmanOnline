#include "hostwindow.h"
#include "ui_hostwindow.h"
#include "boardwindow.h"

HostWindow::HostWindow(QWidget *parent): QDialog(parent)
    , ui(new Ui::HostWindow)
{
    ui->setupUi(this);
}

HostWindow::~HostWindow()
{
    delete ui;
}

void HostWindow::on_okButton_clicked()
{
    QString name = ui->nameInput->text();
    bool ok;
    quint16 port = ui->portInput->text().toUShort(&ok, 10);

    if(!ok || name == "" || port == NULL){
        return;
    }

    QWidget *window = ui->okButton->window();
    BoardWindow* boardWindow = new BoardWindow(name, port, this);

    QWidget *parent = window->parentWidget();

    parent->close();
    window->close();

    boardWindow->show();
}

