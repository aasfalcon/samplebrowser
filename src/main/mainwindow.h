#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QItemSelection>
#include <QMainWindow>
#include <QModelIndex>

#include <engine/player.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent, Player &player);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QModelIndex _last_expanded;
    Player &_player;

private slots:
    void filesSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void on_treeView_expanded(const QModelIndex &index);
    void on_treeView_activated(const QModelIndex &index);
    void on_treeView_clicked(const QModelIndex &index);
};

#endif // MAINWINDOW_H
