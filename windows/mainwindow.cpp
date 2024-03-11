#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include "boardwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap pixmap(":/assets/img/logo.png");

    ui->label->setPixmap(pixmap.scaledToWidth(600));

    QObject::connect(ui->pushButtonHost, &QPushButton::clicked, [&]() {
        QWidget *window = ui->pushButtonHost->window();
        window->close();

        BoardWindow* boardWindow = new BoardWindow(this);
        boardWindow->show();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
