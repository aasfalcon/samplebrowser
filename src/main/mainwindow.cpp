#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileSystemModel>
#include <QDirModel>
#include <QMenuBar>
#include <QTime>
#include <QSettings>
#include <QCloseEvent>

#include "driverdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _scanner(new Scanner(this))
{
    ui->setupUi(this);

    // level meter and slider
    QHBoxLayout *layout = new QHBoxLayout(ui->meterArea);
    ui->meterArea->setLayout(layout);
    _meter = new Meter(ui->meterArea);
    layout->addWidget(_meter);
    layout->addWidget(ui->level);

    _meterUpdater = new Helper(this, true);
    connect(_meterUpdater, &Helper::repeating,
            this, &MainWindow::updateMeter);

    QSettings config;
    ui->level->setValue(config.value("main/level", 80).toInt());

    // dir tree
    QFileSystemModel *model = new QFileSystemModel(ui->treeView);
    model->setFilter(QDir::AllDirs|QDir::NoDotAndDotDot);

    QString home = QDir::homePath();
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
    connect(ui->files->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::filesSelectionChanged);

    // details pane
    ui->details->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->details->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // scanner
    _peaksUpdater = new Helper(this);
    connect(_peaksUpdater, &Helper::performing,
            this, &MainWindow::updatePeaks);
}

MainWindow::~MainWindow()
{
    QSettings config;
    config.sync();
    delete ui;
}

void MainWindow::filesSelectionChanged(const QItemSelection &selected, const QItemSelection &) {
    QFileSystemModel
            *filesModel = (QFileSystemModel *)ui->files->model();
    const QModelIndexList &selectedIndexes = selected.indexes();

    if (selectedIndexes.count()) {
        QModelIndex index = selected.indexes().at(0);
        QString path = filesModel->filePath(index.sibling(index.row(), 0));
        //_player.play(path);

        // show file info
//        SoundFile::TextInfo info = SoundFile::readInfo(path);
//        ui->filename->setText(info.name);
//        ui->details->setItem(0, 0, new QTableWidgetItem(info.format));
//        ui->details->setItem(0, 1, new QTableWidgetItem(info.codec));
//        ui->details->setItem(0, 2, new QTableWidgetItem(info.channels));
//        ui->details->setItem(0, 3, new QTableWidgetItem(info.rate));
//        ui->details->setItem(0, 4, new QTableWidgetItem(info.duration));
//        ui->details->setItem(0, 5, new QTableWidgetItem(info.size));

        //_peaksUpdater->perform(path);
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QSettings config;
    config.setValue("main/level", ui->level->value());
    event->accept();
}

void MainWindow::on_treeView_expanded(const QModelIndex &index)
{
    if (_last_expanded.isValid()) {
        QModelIndexList previous_parents, current_parents;
        QModelIndex walker = _last_expanded;

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

    _last_expanded = QModelIndex(index);
}

void MainWindow::on_treeView_activated(const QModelIndex &index)
{
    ui->treeView->expand(index);

    QFileSystemModel
            *filesModel = (QFileSystemModel *)ui->files->model(),
            *treeModel = (QFileSystemModel *)ui->treeView->model();
    QString path = treeModel->filePath(index);
    _scanner->perform(path);

    filesModel->setRootPath(path);
    ui->files->setRootIndex(filesModel->index(path));
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    on_treeView_activated(index);
}

void MainWindow::on_level_valueChanged(int value)
{
    const double a = 1e-3;
    const double b = 6.908;
    double linear = value / (double)ui->level->maximum();
    double level = a * exp(b * linear);

    if (level < .1) {
        level *= linear * 10;
    }

    //_player.setLevel(level);
}

void MainWindow::updateMeter()
{
    //Sample *peaks = _player.peak();
    //_meter->setLeftValue(peaks[0]);
    //_meter->setRightValue(peaks[1]);
}

void MainWindow::updatePeaks(const QString &path)
{
    QString imagePath = _scanner->scanFile(path);
    QPixmap pixmap = QPixmap(imagePath).scaled(ui->peaks->size());
    ui->peaks->setPixmap(pixmap);
}

void MainWindow::on_actionSound_settings_triggered()
{
    DriverDialog *dialog = new DriverDialog(this);
    dialog->show();
}
