#ifndef HOSTWINDOW_H
#define HOSTWINDOW_H

#include <QDialog>

namespace Ui {
class HostWindow;
}

/**
 * @class HostWindow
 * @brief Klasa reprezentująca okno hosta w grze, dziedzicząca po QDialog.
 *
 * Klasa HostWindow zarządza oknem hosta, umożliwiając inicjalizację i konfigurację serwera gry.
 * Jest odpowiedzialna za przygotowanie interfejsu użytkownika oraz logiki niezbędnej do rozpoczęcia gry jako host.
 */
class HostWindow : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor klasy HostWindow.
     * @param parent Wskaźnik do widgetu nadrzędnego, domyślnie nullptr.
     *
     * Inicjalizuje okno hosta, przygotowując interfejs użytkownika oraz ustawienia początkowe serwera gry.
     */
    explicit HostWindow(QWidget *parent = nullptr);
    ~HostWindow();

private:
    Ui::HostWindow *ui;

private slots:
    void on_okButton_clicked();
};

#endif // HOSTWINDOW_H
