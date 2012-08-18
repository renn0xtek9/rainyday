
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
    void Loadplaylist(QString playlist_path);
    void Loaddir(QString dir_path);		
    void Sync_type_has_changed();
private:
   //******************************************OBJECT
   Ui::MainWindow *ui;
   sync_type get_Sync_type();
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
   QFileDialog* FILE_DIALOG_PLAYLIST;
   QFileDialog* FILE_DIALOG_DIR;
  //******************************************METHOD
   void Setup_ui();
   void Load_icons();
   void Load_sounds();
private slots:
   void Launch_playlist_file_dialog();
   void Launch_dir_file_dialog();
   void Handle_error_signal();
   void Update_the_progressbar();  
};

#endif // MAINWINDOW_H
