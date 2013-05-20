#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

class QSettings;
class QTableWidgetItem;

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

    void on_actionMoveFiles_triggered();

private:
    Ui::MainWindow *ui;
    QSettings *m_settings;

    typedef QMap<QString, struct tm> t_fileTimeMap;
    t_fileTimeMap m_fileTimeMap;

    typedef QMap<QString, QTableWidgetItem*> t_fileStatusMap;
    t_fileStatusMap m_fileStatusMap;
};

#endif // MAINWINDOW_H

// Wunsch-Features:
// * Fortschritts-Balken
// * ...
