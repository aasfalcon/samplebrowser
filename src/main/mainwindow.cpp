#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileSystemModel>
#include <QDirModel>
#include <QStandardPaths>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent, Player &player) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _player(player)
{
    ui->setupUi(this);
    QFileSystemModel *model = new QFileSystemModel(ui->treeView);
    model->setFilter(QDir::AllDirs|QDir::NoDotAndDotDot);

    QString home = QString(QStandardPaths::HomeLocation);
    home = "/home/android";
    QModelIndex index = model->index(home);
    ui->treeView->setStyleSheet( "QTreeView::branch {  border-image: url(none.png); }" );
    ui->treeView->setModel(model);
    ui->treeView->setRootIndex(index);
    model->setRootPath(home);

    for (int i = 2; i < model->columnCount(); i++) {
        ui->treeView->hideColumn(i);
    }

    QHeaderView *header = ui->treeView->header();
    header->setSectionResizeMode(0, QHeaderView::ResizeToContents);

    // files
    model = new QFileSystemModel(ui->files);
    model->setFilter(QDir::Files);
    index = model->index(home);
    ui->files->setModel(model);
    ui->files->setRootIndex(index);
    model->setRootPath(home);

    for (int i = 2; i < model->columnCount(); i++) {
        ui->files->hideColumn(i);
    }

    header = ui->files->header();
    header->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    connect(ui->files->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this,
            SLOT(filesSelectionChanged(const QItemSelection &, const QItemSelection &)));

    // details pane
    header = ui->details->verticalHeader();
    header->setSectionResizeMode(QHeaderView::Fixed);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::filesSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    QFileSystemModel
            *filesModel = (QFileSystemModel *)ui->files->model();
    const QModelIndexList &selectedIndexes = selected.indexes();

    if (selectedIndexes.count()) {
        QModelIndex index = selected.indexes().at(0);
        QString path = filesModel->filePath(index.sibling(index.row(), 0));
        qDebug() << path;
        this->_player.play(path);
    }
}

void MainWindow::on_treeView_expanded(const QModelIndex &index)
{
    if (this->_last_expanded.isValid()) {
        QModelIndexList previous_parents, current_parents;
        QModelIndex walker = this->_last_expanded;

        do {
            previous_parents.append(walker);
            walker = walker.parent();
        } while(walker.isValid());

        walker = index;

        do {
            current_parents.append(walker);
            walker = walker.parent();
        } while(walker.isValid());

        foreach (walker, previous_parents) {
            if (current_parents.indexOf(walker) >= 0) {
                break;
            }

            ui->treeView->collapse(walker);
        }
    }

    this->_last_expanded = QModelIndex(index);
}

void MainWindow::on_treeView_activated(const QModelIndex &index)
{
    ui->treeView->expand(index);

    QFileSystemModel
            *filesModel = (QFileSystemModel *)ui->files->model(),
            *treeModel = (QFileSystemModel *)ui->treeView->model();
    QString path = treeModel->filePath(index);
    filesModel->setRootPath(path);
    ui->files->setRootIndex(filesModel->index(path));
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    this->on_treeView_activated(index);
}
