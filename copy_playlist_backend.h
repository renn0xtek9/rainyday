
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

/**
 * \struct copy_playlist_backend
 * \brief cross platform framework that allow to copy songs of a playlist (from several format) into a folder or a mass storage device
 *  This framework is a full Qt implementation. Currently only m3u are supported. This framework is design for a unidirectional synchronization_type
 * Typically you would use it to sync the songs and playlist of your mp3 player or smartphone from you computer once you have connected it
 * with usb.
 *  
 */

class copy_playlist_backend : public QObject
{
  Q_OBJECT
public:
  //******************************************OBJECT
  copy_playlist_backend(); 			/*!<The constructor of the class*/
  ~copy_playlist_backend();			/*!<The destructor of the class*/
  //******************************************SETORS
  bool set_Playlist_path(QString playlist_path);   /*!<Allow you to define which local playlist file (on the source device) has to be use*/
  bool set_Playlist_path(QDir playlist_path);		/*!<Overloaded function*/
  bool set_Device_path(QString device_path);		/*!<Define the destination directory. This is where all files will land*/
  bool set_Device_path(QDir device_path);		/*!<Overloaded function*/
  bool set_Sync_type(sync_type synchronization_type);	/*!<Define the syncing strategy that has to be used*/
  void set_Embed_m3u_file(bool embed);			/*!<Choose wether a bespoke m3u (with relative file path) has to be created on the device. The m3u file will be created directly in the device path directory*/
  //******************************************ACCESSORS
  QStringList get_Song_list();			/*!<Get the list of songs. Eg "/path/to/song.mp3" becomes "song"*/
  QStringList get_New_path_list();		/*!<Get the list of path (on the destination device) where songs will be copied*/
  QString get_Last_Error();			/*!<Retrieve the last emited error*/
  QString get_Playlist_name();			/*!<Get the name of the playlist. In case of a local file, directly the filename without extension*/
  QString get_Full_dir_name();			/*!<Get the complete path of the destination device*/
  QString get_Dir_name();                        /*!<Get the directory nane of the destination devices*/
  int  get_Numbers_of_track();			/*!<Get the numbers of track that have been found in the playlist*/
  int get_Progress(); 				/*!<Get the numbers of track already copied during the copy operation*/
  //******************************************ACTIONORS
public slots:
  bool Load_playlist();				/*!<Launch the operation of scanning a playlist and resolve path to all founf entries*/
  bool Load_playlist(QString playlist_path); 	/*!<Overloaded function*/
  bool Define_new_path();			/*!<Launch the operation of resolving new path for all song that have to be copier*/
  bool Sync_the_playlist();			/*!<Launch the actual copy operation*/
  //******************************************METHOD
  //********************************1st level method  
signals: 
    void Error_raised();			/*!<Emitted when an error is encountered. This is the alarm bell of the backend*/
    void Progress_changed();			/*!<Emitted when the progress made during the copy operation changed*/
private:
  //******************************************OBJECT
  QVector<QDir> SONG_PATH_LIST;			/*!<The list of path to all songs found in the playlist*/
  QVector<QDir>	NEW_SONG_PATH;			/*!<The list of path where all songs will be copied*/
  QDir* DEVICE_PATH;				/*!<The path in which the user want to sync the songs(where it will be written)*/
  QDir* PLAYLIST_PATH;				/*!<The path to the playlist local file*/
  QFile* PLAYLIST_FILE;				/*!<The object that describe the playlist file itself*/
  QString* LAST_ERROR;				/*!<The last raised error*/
  QTextStream* FILE_STREAM;			/*!<A text stream to read the playlist file*/
  sync_type SYNC_TYPE;				/*!<The current strategy*/
  playlist_type PLAYLIST_TYPE;			/*!<The type of playlist (m3u or xspf or wpl etc)*/
  int PROGRESS;					/*!<The numbers of files already copied*/
  bool PLAYLIST_LOADED_FLAG;			/*!<FLAG: if the playlist have been loaded (i.e path to all songs resolved?)*/
  bool NEW_PATH_UPTODATE_FLAG;			/*!<FLAG: if the new path have been resolved or not*/
  bool EMBED_M3U;				/*!<Define wether an m3u file has to be written or not together with the sync operation!*/
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
  QDir Build_a_new_path(QDir* device_path, QDir* song_path );
  QString Build_a_relative_path_from_here(QDir* device_path, QDir* song_path);
  //low level method********************************
  
};

#endif
