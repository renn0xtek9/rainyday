
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QLabel>
#include <QMainWindow>
#include <QString>
#include <QFileDialog>
#include <QWidget>
#include <QErrorMessage>
#include <QSound>
#include "copy_playlist_backend.h"



namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();   
    void listing_all_files(void);
public slots:
    void Copyplaylist();
    void Loadplaylist();
    void Loaddir();		
    void Sync_type_has_changed();
private:
    Ui::MainWindow *ui;
    sync_type get_Sync_type();

    void Setup_ui();
    void Load_icons();
    void Load_sounds();

    copy_playlist_backend* CP_BCK;
    QSound *CLIC;
    QSound *COPYSOUND;
    QErrorMessage *ERRMSG;
    QIcon *ICON;
    QIcon *ICON_COPY;
    QIcon *ICON_PLAYLIST;
    QIcon *ICON_DIR;
    QDir *DEFAULT_PLAYLIST_DIR;
    QDir *DEFAULT_DEVICE_DIR;

private slots:
    void Handle_error_signal();
    void Update_the_progressbar();
  
    
};

#endif // MAINWINDOW_H
