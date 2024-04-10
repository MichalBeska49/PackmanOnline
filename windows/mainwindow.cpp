#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include "hostwindow.h"
#include "joinwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap pixmap(":/assets/img/logo.png");
    ui->label->setPixmap(pixmap.scaledToWidth(600));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonHost_clicked()
{
    HostWindow* hostWindow = new HostWindow(this);
    hostWindow->show();
}

void MainWindow::on_pushButtonJoin_clicked()
{
    JoinWindow* joinWindow = new JoinWindow(this);
    joinWindow->show();
}
