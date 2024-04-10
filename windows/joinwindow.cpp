#include "joinwindow.h"
#include "ui_joinwindow.h"
#include "boardwindow.h"

JoinWindow::JoinWindow(QWidget *parent): QDialog(parent)
    , ui(new Ui::JoinWindow)
{
    ui->setupUi(this);
}

JoinWindow::~JoinWindow()
{
    delete ui;
}

void JoinWindow::on_okButton_clicked()
{
    QString name = ui->nameInput->text();
    bool ok;
    quint16 port = ui->portInput->text().toUShort(&ok, 10);
    QString stringAddress  = ui->addressInput->text();
    QHostAddress address(stringAddress);

    if(!ok || name == "" || address.isNull() || port == NULL){
        return;
    }

    QWidget *window = ui->okButton->window();
    BoardWindow* boardWindow = new BoardWindow(name, address, port, this);

    QWidget *parent = window->parentWidget();

    parent->close();
    window->close();

    boardWindow->show();
}
