#include "hostwindow.h"
#include "ui_hostwindow.h"
#include "boardwindow.h"

HostWindow::HostWindow(QWidget *parent)
    : QDialog(parent)
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
    QString address = ui->addressInput->text();

    if(name != "" && address != ""){
        QTcpServer* server = new QTcpServer(this);
    }
    QWidget *window = ui->okButton->window();
    BoardWindow* boardWindow = new BoardWindow(this);

    QWidget *parent = window->parentWidget();

    boardWindow->show();
    parent->close();
    window->close();
}

