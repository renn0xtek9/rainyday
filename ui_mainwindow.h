/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sat 9. Oct 12:07:39 2010
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *button_playlist;
    QPushButton *button_dir;
    QPushButton *button_copy;
    QLabel *playlist_name;
    QLabel *dir_name;
    QLabel *trcknb;
    QListWidget *songslistview;
    QCheckBox *sub_dir_box;
    QProgressBar *copy_progressbar;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        button_playlist = new QPushButton(centralWidget);
        button_playlist->setObjectName(QString::fromUtf8("button_playlist"));
        button_playlist->setGeometry(QRect(20, 10, 75, 23));
        button_dir = new QPushButton(centralWidget);
        button_dir->setObjectName(QString::fromUtf8("button_dir"));
        button_dir->setGeometry(QRect(20, 60, 75, 23));
        button_copy = new QPushButton(centralWidget);
        button_copy->setObjectName(QString::fromUtf8("button_copy"));
        button_copy->setGeometry(QRect(20, 110, 75, 23));
        playlist_name = new QLabel(centralWidget);
        playlist_name->setObjectName(QString::fromUtf8("playlist_name"));
        playlist_name->setGeometry(QRect(20, 40, 511, 16));
        dir_name = new QLabel(centralWidget);
        dir_name->setObjectName(QString::fromUtf8("dir_name"));
        dir_name->setGeometry(QRect(20, 90, 511, 16));
        trcknb = new QLabel(centralWidget);
        trcknb->setObjectName(QString::fromUtf8("trcknb"));
        trcknb->setGeometry(QRect(20, 140, 511, 16));
        songslistview = new QListWidget(centralWidget);
        songslistview->setObjectName(QString::fromUtf8("songslistview"));
        songslistview->setGeometry(QRect(20, 170, 521, 221));
        sub_dir_box = new QCheckBox(centralWidget);
        sub_dir_box->setObjectName(QString::fromUtf8("sub_dir_box"));
        sub_dir_box->setGeometry(QRect(120, 110, 161, 17));
        copy_progressbar = new QProgressBar(centralWidget);
        copy_progressbar->setObjectName(QString::fromUtf8("copy_progressbar"));
        copy_progressbar->setGeometry(QRect(20, 400, 511, 23));
        copy_progressbar->setValue(24);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 551, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        button_playlist->setText(QApplication::translate("MainWindow", "Playlist", 0, QApplication::UnicodeUTF8));
        button_dir->setText(QApplication::translate("MainWindow", "Directory", 0, QApplication::UnicodeUTF8));
        button_copy->setText(QApplication::translate("MainWindow", "Copy", 0, QApplication::UnicodeUTF8));
        playlist_name->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
        dir_name->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
        trcknb->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
        sub_dir_box->setText(QApplication::translate("MainWindow", "Create it own subdirectory", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
