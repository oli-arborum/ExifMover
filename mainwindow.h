#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QSettings;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pbSelectDir_clicked();

    void on_actionReadDir_triggered();

    void on_actionClearList_triggered();

private:
    Ui::MainWindow *ui;
    QSettings *m_settings;
};

#endif // MAINWINDOW_H

// Wunsch-Features:
// * Fortschritts-Balken
// * ...
