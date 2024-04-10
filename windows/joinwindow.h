#ifndef JOINWINDOW_H
#define JOINWINDOW_H

#include <QDialog>
#include <QHostAddress>

namespace Ui {
class JoinWindow;
}

/**
 * @class JoinWindow
 * @brief Klasa reprezentująca okno dołączania do gry, dziedzicząca po QDialog.
 *
 * Klasa JoinWindow zarządza oknem umożliwiającym dołączenie do istniejącej gry jako klient.
 * Jest odpowiedzialna za prezentację interfejsu użytkownika, który pozwala użytkownikowi
 * wprowadzić dane niezbędne do połączenia się z serwerem hostującym grę, takie jak adres IP i port.
 */
class JoinWindow : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor klasy JoinWindow.
     * @param parent Wskaźnik do widgetu nadrzędnego, domyślnie nullptr.
     *
     * Inicjalizuje okno dołączania, przygotowując interfejs użytkownika oraz
     * opcje konfiguracyjne niezbędne do nawiązania połączenia z serwerem gry.
     */
    explicit JoinWindow(QWidget *parent = nullptr);
    ~JoinWindow();

private:
    Ui::JoinWindow *ui;

private slots:
    void on_okButton_clicked();
};

#endif // JOINWINDOW_H
