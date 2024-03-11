#ifndef BOARDWINDOW_H
#define BOARDWINDOW_H

#include <QDialog>

namespace Ui {
class BoardWindow;
}

class BoardWindow : public QDialog
{
    Q_OBJECT

public:
    explicit BoardWindow(QWidget *parent = nullptr);
    ~BoardWindow();

private:
    Ui::BoardWindow *ui;
};

#endif // BOARDWINDOW_H
