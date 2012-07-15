
#ifndef COPY_PLAYLIST_BACKEND	
#define COPY_PLAYLIST_BACKEND
#include<QDir>
#include<QVector>
#include<QFile>
#include<QString>
#include<QStringList>
#include <QTextStream>



enum sync_type {flat, keep_arch, one_folder_per_playlist};
enum playlist_type{m3u, xspf, wpl, amarok_db, unknown};


class copy_playlist_backend : public QObject
{
  Q_OBJECT
public:
  //******************************************OBJECT
  copy_playlist_backend();
  ~copy_playlist_backend();
  //******************************************SETORS
  bool set_Playlist_path(QString playlist_path);
  bool set_Playlist_path(QDir playlist_path);
  bool set_Device_path(QString device_path);
  bool set_Device_path(QDir device_path);
  bool set_Sync_type(sync_type synchronization_type);
  void set_Embed_m3u_file(bool embed);
  //******************************************ACCESSORS
  QStringList get_Song_list();
  QStringList get_New_path_list();
  QString get_Last_Error();
  QString get_Playlist_name();		//return name without extension
  QString get_Full_dir_name();
  QString get_Dir_name();
  int  get_Numbers_of_track();
  int get_Progress();
  //******************************************ACTIONORS
public slots:
  bool Load_playlist();		//Assume the playlist_path have been given through set_Playlist_path before
  bool Load_playlist(QString playlist_path);
  bool Define_new_path();
  bool Sync_the_playlist();
  //******************************************METHOD
  //********************************1st level method  
signals: 
    void Error_raised();
    void Progress_changed();
private:
  //******************************************OBJECT
  QVector<QDir> SONG_PATH_LIST;		
  QVector<QDir>	NEW_SONG_PATH;		//store absolute path of the new songs to be stored on the device
  QDir* DEVICE_PATH;
  QDir* PLAYLIST_PATH;
  QFile* PLAYLIST_FILE;
  QString* LAST_ERROR;
  QTextStream* FILE_STREAM;
  sync_type SYNC_TYPE;
  playlist_type PLAYLIST_TYPE;
  int PROGRESS;					//Progress of the copy process from 0 to 1
  bool PLAYLIST_LOADED_FLAG;
  bool NEW_PATH_UPTODATE_FLAG;
  bool EMBED_M3U;
  //******************************************METHOD
  //********************************1st level method
  bool List_all_files();	//A0
  bool Define_new_m3u();				//C0
  bool Autodetect_playlist_type();			//E0//
  bool is_Playlist_path_valid();			// check validity of path (if the path exist, is readable? not if it is a known kind of playlist
  bool is_Device_path_valid();				// check validity of path (if the path exist, is writable?)
  bool is_Valid_song_path(QDir song_path);
  //*******************2nd level method*************
  bool List_all_files_from_m3u();		
  bool List_all_files_from_xspf();			
  bool List_all_files_from_wpl();			
  bool List_all_files_from_amarok_db();		
  //********3rd level method************************
  bool Open_local_playlist_file();			//A011 -A021 -A031
  bool Go_to_the_device_path_position();
  bool Go_to_playlist_path_position();			//Place the current path to the directorie in which to playlist is stored
  bool Go_to_the_song_position(QDir* song_path);	//Plaece the current path to the dir in which the song is stored
  QString Get_the_following_record();			//A012
  QString Get_name_of_file_from_path(QDir* dir);
  QDir Create_absolute_path();				//A014
  QDir Build_a_new_path(QDir* device_path, QDir* song_path );
  QString Build_a_relative_path_from_here(QDir* device_path, QDir* song_path);
  //low level method********************************
  bool Add_path_to_list();				//A015
  
};

#endif
