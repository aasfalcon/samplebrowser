#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QItemSelection>
#include <QMainWindow>
#include <QModelIndex>

#include "engine/helper.h"
#include "engine/scanner.h"
#include "widgets/meter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent*);

private:
    Ui::MainWindow* ui;
    QModelIndex _last_expanded;
    Scanner* _scanner;
    friend class PeaksUpdater;
    //class PeaksUpdater *_peaks_updater;
    Helper* _meterUpdater;
    Helper* _peaksUpdater;

    Meter* _meter;
    void updatePeaks(const QString& path);

private slots:
    void filesSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
    void on_treeView_expanded(const QModelIndex& index);
    void on_treeView_activated(const QModelIndex& index);
    void on_treeView_clicked(const QModelIndex& index);
    void on_level_valueChanged(int value);
    void updateMeter();
    void on_actionSound_settings_triggered();
};

#endif // MAINWINDOW_H
