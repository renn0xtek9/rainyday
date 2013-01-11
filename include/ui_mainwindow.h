/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Thu Nov 8 15:44:31 2012
**      by: Qt User Interface Compiler version 4.8.2
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
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QWidget *gather_top;
    QHBoxLayout *horizontalLayout;
    QWidget *button_column;
    QLabel *playlist_name;
    QLabel *dir_name;
    QPushButton *button_playlist;
    QPushButton *button_copy;
    QLabel *trcknb;
    QPushButton *button_dir;
    QWidget *parameter_column;
    QCheckBox *Embed_m3u_box;
    QComboBox *sync_type_box;
    QLineEdit *lineEdit_music_mother_dir;
    QLabel *label_music_mother_dir;
    QWidget *gather_lists;
    QHBoxLayout *horizontalLayout_2;
    QListWidget *songslistview;
    QListWidget *dirlist_view;
    QProgressBar *copy_progressbar;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(494, 500);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 471, 441));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        gather_top = new QWidget(verticalLayoutWidget);
        gather_top->setObjectName(QString::fromUtf8("gather_top"));
        gather_top->setMinimumSize(QSize(400, 180));
        horizontalLayout = new QHBoxLayout(gather_top);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        button_column = new QWidget(gather_top);
        button_column->setObjectName(QString::fromUtf8("button_column"));
        playlist_name = new QLabel(button_column);
        playlist_name->setObjectName(QString::fromUtf8("playlist_name"));
        playlist_name->setGeometry(QRect(20, 40, 511, 20));
        dir_name = new QLabel(button_column);
        dir_name->setObjectName(QString::fromUtf8("dir_name"));
        dir_name->setGeometry(QRect(20, 100, 400, 20));
        button_playlist = new QPushButton(button_column);
        button_playlist->setObjectName(QString::fromUtf8("button_playlist"));
        button_playlist->setGeometry(QRect(0, 0, 100, 40));
        button_playlist->setFlat(false);
        button_copy = new QPushButton(button_column);
        button_copy->setObjectName(QString::fromUtf8("button_copy"));
        button_copy->setGeometry(QRect(0, 120, 100, 40));
        trcknb = new QLabel(button_column);
        trcknb->setObjectName(QString::fromUtf8("trcknb"));
        trcknb->setGeometry(QRect(20, 160, 40, 20));
        button_dir = new QPushButton(button_column);
        button_dir->setObjectName(QString::fromUtf8("button_dir"));
        button_dir->setGeometry(QRect(0, 60, 100, 40));

        horizontalLayout->addWidget(button_column);

        parameter_column = new QWidget(gather_top);
        parameter_column->setObjectName(QString::fromUtf8("parameter_column"));
        Embed_m3u_box = new QCheckBox(parameter_column);
        Embed_m3u_box->setObjectName(QString::fromUtf8("Embed_m3u_box"));
        Embed_m3u_box->setGeometry(QRect(0, 87, 111, 21));
        Embed_m3u_box->setChecked(true);
        sync_type_box = new QComboBox(parameter_column);
        sync_type_box->setObjectName(QString::fromUtf8("sync_type_box"));
        sync_type_box->setGeometry(QRect(0, 120, 180, 22));
        lineEdit_music_mother_dir = new QLineEdit(parameter_column);
        lineEdit_music_mother_dir->setObjectName(QString::fromUtf8("lineEdit_music_mother_dir"));
        lineEdit_music_mother_dir->setGeometry(QRect(1, 62, 181, 21));
        label_music_mother_dir = new QLabel(parameter_column);
        label_music_mother_dir->setObjectName(QString::fromUtf8("label_music_mother_dir"));
        label_music_mother_dir->setGeometry(QRect(5, 38, 171, 16));

        horizontalLayout->addWidget(parameter_column);


        verticalLayout->addWidget(gather_top);

        gather_lists = new QWidget(verticalLayoutWidget);
        gather_lists->setObjectName(QString::fromUtf8("gather_lists"));
        horizontalLayout_2 = new QHBoxLayout(gather_lists);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        songslistview = new QListWidget(gather_lists);
        songslistview->setObjectName(QString::fromUtf8("songslistview"));

        horizontalLayout_2->addWidget(songslistview);

        dirlist_view = new QListWidget(gather_lists);
        dirlist_view->setObjectName(QString::fromUtf8("dirlist_view"));

        horizontalLayout_2->addWidget(dirlist_view);


        verticalLayout->addWidget(gather_lists);

        copy_progressbar = new QProgressBar(verticalLayoutWidget);
        copy_progressbar->setObjectName(QString::fromUtf8("copy_progressbar"));
        copy_progressbar->setValue(24);

        verticalLayout->addWidget(copy_progressbar);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 494, 19));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        QWidget::setTabOrder(songslistview, button_copy);
        QWidget::setTabOrder(button_copy, button_playlist);
        QWidget::setTabOrder(button_playlist, button_dir);
        QWidget::setTabOrder(button_dir, sync_type_box);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        playlist_name->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
        dir_name->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
        button_playlist->setText(QApplication::translate("MainWindow", "Playlist", 0, QApplication::UnicodeUTF8));
        button_copy->setText(QApplication::translate("MainWindow", "Copy", 0, QApplication::UnicodeUTF8));
        trcknb->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
        button_dir->setText(QApplication::translate("MainWindow", "Directory", 0, QApplication::UnicodeUTF8));
        Embed_m3u_box->setText(QApplication::translate("MainWindow", "Embed m3u file ?", 0, QApplication::UnicodeUTF8));
        label_music_mother_dir->setText(QApplication::translate("MainWindow", "Enter path of mother music dir ", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
