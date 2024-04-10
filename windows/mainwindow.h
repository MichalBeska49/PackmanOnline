#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief Główne okno aplikacji, dziedziczące po QMainWindow.
 *
 * Klasa MainWindow stanowi główne okno interfejsu użytkownika aplikacji.
 * Zarządza prezentacją menu startowego,
 * opcjami gry oraz przejściami między różnymi oknami aplikacji,
 * okno dołączania do gry, i okno hostowania gry.
 * Jest to centrum interakcji użytkownika z aplikacją.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonHost_clicked();
    void on_pushButtonJoin_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
