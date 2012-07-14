#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QMainWindow>
#include <QString>
#include <QFileDialog>
#include <QWidget>
#include <QErrorMessage>
#include <QSound>
#include "copy_playlist_backend.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){    
    ui->setupUi(this);
    Load_icons();
    Load_sounds();
    Setup_ui();    
    ERRMSG=new QErrorMessage(this);
    CP_BCK=new copy_playlist_backend;
    DEFAULT_PLAYLIST_DIR=new QDir(QDir::home());
    DEFAULT_DEVICE_DIR=new QDir(QDir::root());
    //Connections
    QObject::connect(ui->button_copy,SIGNAL(clicked()),this,SLOT(Copyplaylist()));
    QObject::connect(ui->button_playlist,SIGNAL(clicked()),this,SLOT(Loadplaylist()));
    QObject::connect(ui->button_dir,SIGNAL(clicked()),this,SLOT(Loaddir()));    
    QObject::connect(CP_BCK,SIGNAL(Error_raised()),this,SLOT(Handle_error_signal()));
    QObject::connect(CP_BCK,SIGNAL(Progress_changed()),this,SLOT(Update_the_progressbar()));
}

MainWindow::~MainWindow(){
    delete ui;
    //TODO DELETE AND DESTRUCT OTHER OBJECT

}
void MainWindow::Copyplaylist(){           //copy the content of the playlist to a new directory
    CLIC->play();
    CP_BCK->Sync_the_playlist();
    //TODO add a song for when it's done 
}
void MainWindow::Loadplaylist(){
  CLIC->play();
  CP_BCK->Load_playlist(QFileDialog::getOpenFileName(this,"Open playlist",DEFAULT_PLAYLIST_DIR->path(),"Playlist (*m3u)")); 
}
void MainWindow::Loaddir(){
  CLIC->play();
  CP_BCK->set_Device_path(QFileDialog::getExistingDirectory(this,"Select the device path",DEFAULT_DEVICE_DIR->path(),QFileDialog::ShowDirsOnly));
  CP_BCK->Define_new_path();
}
void MainWindow::Setup_ui(){
    Load_icons();
    setWindowTitle("QPlaylist Transfer");
    setWindowIcon(*ICON);
    setFixedSize(600,500);
    ui->button_copy->setIcon(*ICON_COPY);
    ui->button_playlist->setIcon(*ICON_PLAYLIST);
    ui->button_dir->setIcon(*ICON_DIR);
    ui->copy_progressbar->setValue(0);
    ui->copy_progressbar->setMaximum(1);
    ui->copy_progressbar->setMinimum(0);
}
void MainWindow::Load_icons(){
  ICON =new QIcon("icon.ico");
  ICON_COPY=new QIcon("copy.png");
  ICON_PLAYLIST=new QIcon("playlist.png");
  ICON_DIR=new QIcon("dir.png");
}
void MainWindow::Load_sounds(){
  CLIC=new QSound("clic.wav");
  COPYSOUND=new QSound("copy.wav");
}
void MainWindow::Update_the_progressbar(){
  ui->copy_progressbar->setMaximum(CP_BCK->get_Numbers_of_track());
  ui->copy_progressbar->setValue(CP_BCK->get_Progress());

}

void MainWindow::Handle_error_signal(){
  ERRMSG->showMessage(CP_BCK->get_Last_Error());
}





