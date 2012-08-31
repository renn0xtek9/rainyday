#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QLabel>
#include <QMainWindow>
#include <QString>
#include <QFileDialog>
#include <QWidget>
#include <QErrorMessage>
#include <QSound>
#include <QMessageBox>
#include "copy_playlist_backend.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){    
    ui->setupUi(this);
    Load_sounds();
    Setup_ui();    
    ERRMSG=new QErrorMessage(this);
    CP_BCK=new copy_playlist_backend;
    DIALOG_COPY_ENDED=new QMessageBox;
    DIALOG_COPY_ENDED->setWindowIcon(*ICON);
    DIALOG_COPY_ENDED->setIcon(QMessageBox::Information);
    DIALOG_COPY_ENDED->setText("Copy operation is finished");
    OK_BUTTON=DIALOG_COPY_ENDED->addButton("Ok",QMessageBox::AcceptRole);
    DEFAULT_PLAYLIST_DIR=new QDir(QDir::home());
    DEFAULT_DEVICE_DIR=new QDir(QDir::root());
    FILE_DIALOG_PLAYLIST=new QFileDialog(this,"Open a playlist",DEFAULT_PLAYLIST_DIR->path(),"all/allfiles");
    FILE_DIALOG_PLAYLIST->setNameFilter("All playlist files (*.m3u *.xspf *wpl)");
    FILE_DIALOG_DIR=new QFileDialog(this,"Select a directory",DEFAULT_DEVICE_DIR->path(),"all/allfiles");
    FILE_DIALOG_DIR->setFilter(QDir::AllDirs);
    FILE_DIALOG_DIR->setFileMode(QFileDialog::Directory);
    
    //Initial conditions
    CP_BCK->set_Sync_type(get_Sync_type());
    //Connections
    QObject::connect(ui->button_copy,SIGNAL(clicked()),this,SLOT(Copyplaylist()));
    QObject::connect(ui->button_playlist,SIGNAL(clicked()),this,SLOT(Launch_playlist_file_dialog()));
    QObject::connect(ui->button_dir,SIGNAL(clicked()),this,SLOT(Launch_dir_file_dialog()));   
    QObject::connect(FILE_DIALOG_PLAYLIST,SIGNAL(fileSelected(QString)),this,SLOT(Loadplaylist(QString)));
    QObject::connect(FILE_DIALOG_DIR,SIGNAL(fileSelected(QString)),this,SLOT(Loaddir(QString)));
    QObject::connect(CP_BCK,SIGNAL(Error_raised()),this,SLOT(Handle_error_signal()));
    QObject::connect(CP_BCK,SIGNAL(Progress_changed()),this,SLOT(Update_the_progressbar()));
    QObject::connect(ui->sync_type_box,SIGNAL(currentIndexChanged(int)),this,SLOT(Sync_type_has_changed()));
    QObject::connect(CP_BCK,SIGNAL(Copy_operation_ended()),DIALOG_COPY_ENDED,SLOT(show()));
    QObject::connect(OK_BUTTON,SIGNAL(clicked(bool)),CP_BCK,SLOT(Roger_the_notification_of_end_of_operation()));
    QObject::connect(CP_BCK,SIGNAL(A_new_playlist_is_loaded()),this,SLOT(Retrieve_playlist_song()));
    QObject::connect(CP_BCK,SIGNAL(The_dir_is_uptodate()),this,SLOT(Retrieve_new_path()));
    
    
    ui->button_copy->setFlat(false);
    ui->button_dir->setFlat(true);
    ui->button_playlist->setFlat(false);
    }
MainWindow::~MainWindow(){
    delete ui;
    //TODO DELETE AND DESTRUCT OTHER OBJECT

}
void MainWindow::Copyplaylist(){           //copy the content of the playlist to a new directory
    CLIC->play();
    CP_BCK->set_Embed_m3u_file(ui->Embed_m3u_box->isChecked());
    CP_BCK->Sync_the_playlist();
    //TODO add a song for when it's done 
}
void MainWindow::Loadplaylist(QString playlist_path){
  CP_BCK->Load_playlist(playlist_path); 
}
void MainWindow::Retrieve_playlist_song(){
  ui->songslistview->clear();
  ui->songslistview->addItems(CP_BCK->get_Song_list());
  ui->playlist_name->setText(CP_BCK->get_Playlist_name());
  ui->trcknb->setNum(CP_BCK->get_Numbers_of_track());  
}
void MainWindow::Retrieve_new_path(){
  ui->dirlist_view->clear();
  ui->dir_name->setText(CP_BCK->get_Full_dir_name());
  ui->dirlist_view->addItems(CP_BCK->get_New_path_list());
}
void MainWindow::Sync_type_has_changed(){  
  CP_BCK->set_Sync_type(get_Sync_type());
}
void MainWindow::Loaddir(QString dir_path){
  CP_BCK->set_Device_path(dir_path);
  CP_BCK->Define_new_path();
}
void MainWindow::Setup_ui(){
    Load_icons();
    setWindowTitle("QPlaylist Transfer");
    setWindowIcon(*ICON);
    setFixedSize(430,500);
    ui->button_copy->setIcon(*ICON_COPY);
    ui->button_playlist->setIcon(*ICON_PLAYLIST);
    ui->button_dir->setIcon(*ICON_DIR);
    ui->copy_progressbar->setValue(0);
    ui->copy_progressbar->setMaximum(1);
    ui->copy_progressbar->setMinimum(0);
    ui->dir_name->setText("");
    ui->playlist_name->setText("");
    ui->trcknb->setNum(0);
    ui->sync_type_box->addItem("Keep file architecture");
    ui->sync_type_box->addItem("One folder per playlist");
    ui->sync_type_box->addItem("Flat synchronisation");
    ui->sync_type_box->setFixedWidth(200);
    

}
void MainWindow::Load_icons(){
  ICON =new QIcon(":rainyday_64.png");
  ICON_COPY=new QIcon(":copy.png");
  ICON_PLAYLIST=new QIcon(":playlist.png");
  qDebug()<<"ICON PLAYLIST INVALID?"<<ICON_PLAYLIST->isNull();
  ICON_DIR=new QIcon(":dir.png");
}
void MainWindow::Load_sounds(){
  CLIC=new QSound(":clic.wav");
  COPYSOUND=new QSound(":copy.wav");
}
void MainWindow::Update_the_progressbar(){
  if (CP_BCK->get_Numbers_of_track()>1){
  ui->copy_progressbar->setMaximum(CP_BCK->get_Numbers_of_track());
  }
  else {
    ui->copy_progressbar->setMaximum(1);  //at least 1 as max of the progressbar so that it remains 0% when no song loaded
  }
  ui->copy_progressbar->setValue(CP_BCK->get_Progress());
}
void MainWindow::Handle_error_signal(){
  ERRMSG->showMessage(CP_BCK->get_Last_Error());
}
sync_type MainWindow::get_Sync_type(){
  sync_type result;
  switch (ui->sync_type_box->currentIndex()){
    case(0):{
      result=keep_arch;
      break;
    }
    case(1):{
      result=one_folder_per_playlist;
      break;
    }
    case(2):{
      result=flat;
    }
  }
  return result;
}
void MainWindow::Launch_dir_file_dialog(){
  CLIC->play();
  FILE_DIALOG_DIR->show();
}
void MainWindow::Launch_playlist_file_dialog(){
  CLIC->play(); 
  FILE_DIALOG_PLAYLIST->show();
}





