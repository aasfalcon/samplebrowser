/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QWidget *browserArea;
    QHBoxLayout *horizontalLayout;
    QTreeView *treeView;
    QTreeView *files;
    QWidget *infoArea;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QLabel *filename;
    QTableWidget *details;
    QGroupBox *groupBox_2;
    QWidget *editorArea;
    QHBoxLayout *horizontalLayout_2;
    QLabel *peaks;
    QWidget *meterArea;
    QSlider *level;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(800, 600);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        browserArea = new QWidget(centralWidget);
        browserArea->setObjectName(QStringLiteral("browserArea"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(browserArea->sizePolicy().hasHeightForWidth());
        browserArea->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(browserArea);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        treeView = new QTreeView(browserArea);
        treeView->setObjectName(QStringLiteral("treeView"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(treeView->sizePolicy().hasHeightForWidth());
        treeView->setSizePolicy(sizePolicy1);
        treeView->setMinimumSize(QSize(250, 0));
        treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        treeView->setProperty("showDropIndicator", QVariant(false));
        treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
        treeView->setIconSize(QSize(24, 24));
        treeView->setAutoExpandDelay(100);
        treeView->setRootIsDecorated(false);
        treeView->setUniformRowHeights(true);
        treeView->setAnimated(true);
        treeView->setAllColumnsShowFocus(true);
        treeView->setHeaderHidden(true);
        treeView->setExpandsOnDoubleClick(false);

        horizontalLayout->addWidget(treeView);

        files = new QTreeView(browserArea);
        files->setObjectName(QStringLiteral("files"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(2);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(files->sizePolicy().hasHeightForWidth());
        files->setSizePolicy(sizePolicy2);
        files->setAlternatingRowColors(true);
        files->setIconSize(QSize(32, 32));
        files->setRootIsDecorated(false);
        files->setItemsExpandable(false);
        files->setSortingEnabled(true);
        files->setExpandsOnDoubleClick(false);

        horizontalLayout->addWidget(files);

        infoArea = new QWidget(browserArea);
        infoArea->setObjectName(QStringLiteral("infoArea"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(1);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(infoArea->sizePolicy().hasHeightForWidth());
        infoArea->setSizePolicy(sizePolicy3);
        infoArea->setMinimumSize(QSize(250, 0));
        infoArea->setMaximumSize(QSize(300, 16777215));
        verticalLayout_4 = new QVBoxLayout(infoArea);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        groupBox = new QGroupBox(infoArea);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setStyleSheet(QStringLiteral(""));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 12, 0, 0);
        filename = new QLabel(groupBox);
        filename->setObjectName(QStringLiteral("filename"));
        filename->setWordWrap(true);
        filename->setMargin(4);

        verticalLayout_2->addWidget(filename);

        details = new QTableWidget(groupBox);
        if (details->columnCount() < 1)
            details->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        details->setHorizontalHeaderItem(0, __qtablewidgetitem);
        if (details->rowCount() < 6)
            details->setRowCount(6);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setTextAlignment(Qt::AlignTrailing|Qt::AlignVCenter);
        details->setVerticalHeaderItem(0, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        __qtablewidgetitem2->setTextAlignment(Qt::AlignTrailing|Qt::AlignVCenter);
        details->setVerticalHeaderItem(1, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        __qtablewidgetitem3->setTextAlignment(Qt::AlignTrailing|Qt::AlignVCenter);
        details->setVerticalHeaderItem(2, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        __qtablewidgetitem4->setTextAlignment(Qt::AlignTrailing|Qt::AlignVCenter);
        details->setVerticalHeaderItem(3, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        __qtablewidgetitem5->setTextAlignment(Qt::AlignTrailing|Qt::AlignVCenter);
        details->setVerticalHeaderItem(4, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        __qtablewidgetitem6->setTextAlignment(Qt::AlignTrailing|Qt::AlignVCenter);
        details->setVerticalHeaderItem(5, __qtablewidgetitem6);
        details->setObjectName(QStringLiteral("details"));
        details->setFrameShape(QFrame::NoFrame);
        details->setLineWidth(0);
        details->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        details->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        details->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        details->setAutoScroll(false);
        details->setTabKeyNavigation(false);
        details->setProperty("showDropIndicator", QVariant(false));
        details->setSelectionMode(QAbstractItemView::NoSelection);
        details->setShowGrid(false);
        details->setCornerButtonEnabled(false);
        details->horizontalHeader()->setVisible(false);
        details->horizontalHeader()->setHighlightSections(false);
        details->verticalHeader()->setHighlightSections(false);

        verticalLayout_2->addWidget(details);


        verticalLayout_4->addWidget(groupBox);

        groupBox_2 = new QGroupBox(infoArea);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));

        verticalLayout_4->addWidget(groupBox_2);


        horizontalLayout->addWidget(infoArea);


        verticalLayout->addWidget(browserArea);

        editorArea = new QWidget(centralWidget);
        editorArea->setObjectName(QStringLiteral("editorArea"));
        sizePolicy.setHeightForWidth(editorArea->sizePolicy().hasHeightForWidth());
        editorArea->setSizePolicy(sizePolicy);
        editorArea->setMinimumSize(QSize(0, 200));
        editorArea->setMaximumSize(QSize(16777215, 200));
        horizontalLayout_2 = new QHBoxLayout(editorArea);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        peaks = new QLabel(editorArea);
        peaks->setObjectName(QStringLiteral("peaks"));
        peaks->setScaledContents(true);

        horizontalLayout_2->addWidget(peaks);

        meterArea = new QWidget(editorArea);
        meterArea->setObjectName(QStringLiteral("meterArea"));
        meterArea->setMaximumSize(QSize(100, 16777215));
        level = new QSlider(meterArea);
        level->setObjectName(QStringLiteral("level"));
        level->setGeometry(QRect(37, 0, 25, 84));
        level->setFocusPolicy(Qt::NoFocus);
        level->setMaximum(100);
        level->setValue(50);
        level->setInvertedControls(false);
        level->setTickPosition(QSlider::TicksBelow);

        horizontalLayout_2->addWidget(meterArea);


        verticalLayout->addWidget(editorArea);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "WAVExplorer", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "File details", 0));
        filename->setText(QString());
        QTableWidgetItem *___qtablewidgetitem = details->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "Value", 0));
        QTableWidgetItem *___qtablewidgetitem1 = details->verticalHeaderItem(0);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "Format:", 0));
        QTableWidgetItem *___qtablewidgetitem2 = details->verticalHeaderItem(1);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "Codec:", 0));
        QTableWidgetItem *___qtablewidgetitem3 = details->verticalHeaderItem(2);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "Channels:", 0));
        QTableWidgetItem *___qtablewidgetitem4 = details->verticalHeaderItem(3);
        ___qtablewidgetitem4->setText(QApplication::translate("MainWindow", "Sample rate:", 0));
        QTableWidgetItem *___qtablewidgetitem5 = details->verticalHeaderItem(4);
        ___qtablewidgetitem5->setText(QApplication::translate("MainWindow", "Duration:", 0));
        QTableWidgetItem *___qtablewidgetitem6 = details->verticalHeaderItem(5);
        ___qtablewidgetitem6->setText(QApplication::translate("MainWindow", "Size:", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Library data", 0));
        peaks->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
