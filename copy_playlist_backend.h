
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
  bool Load_playlist();				/*!<Launch the operation of scanning a playlist and resolve path to all found entries*/
  bool Load_playlist(QString playlist_path); 	/*!<Overloaded function*/
  bool Define_new_path();			/*!<Launch the operation of resolving new path for all song that have to be copier*/
  bool Sync_the_playlist();			/*!<Launch the actual copy operation*/
  void Roger_the_notification_of_end_of_operation(); /*!<Tell the backend we understand the copy operation is ended. This will re-put the progress bar to 0 and SUCCESS to false*/
  
  //******************************************METHOD
  //********************************1st level method  
signals: 
    void Error_raised();			/*!<Emitted when an error is encountered. This is the alarm bell of the backend*/
    void Progress_changed();			/*!<Emitted when the progress made during the copy operation changed*/
    void Copy_operation_ended();		/*!<Emitted when thes copying operation is ended, no matter succesfuly or not*/
    void A_new_playlist_is_loaded();		/*!<Emitted if the path registered in a playlist have been successfully loaded*/
    void The_dir_is_uptodate();			/*!<Emitted if the new path have been succesfully built*/
private:
  //******************************************OBJECT
  QVector<QDir> SONG_PATH_LIST;			/*!<The list of path to all songs found in the playlist*/
  QVector<QDir>	NEW_SONG_PATH;			/*!<The list of path where all songs will be copied*/
  QDir* DEVICE_PATH;				/*!<The path in which the user want to sync the songs(where it will be written)*/
  QDir* PLAYLIST_PATH;				/*!<The path to the playlist local file*/
  QFile* PLAYLIST_FILE;				/*!<The object that describe the playlist file itself*/
  QString* LAST_ERROR;				/*!<The last raised error*/
  QTextStream* FILE_STREAM;			/*!<A text stream to read the playlist file*/
  QFile* XQUERY_FILE;				/*!<The file that contains the prepared xquery statement to select all track locations form an xspf file*/
  QString* XSPF_QUERY_ALL_TRACK;		/*!<A string that contains the xquery that will ask for all the track located inside the xspf playlist*/
  sync_type SYNC_TYPE;				/*!<The current strategy*/
  playlist_type PLAYLIST_TYPE;			/*!<The type of playlist (m3u or xspf or wpl etc)*/
  int PROGRESS;					/*!<The numbers of files already copied*/
  bool PLAYLIST_LOADED_FLAG;			/*!<FLAG: if the playlist have been loaded (i.e path to all songs resolved?)*/
  bool NEW_PATH_UPTODATE_FLAG;			/*!<FLAG: if the new path have been resolved or not*/
  bool EMBED_M3U;				/*!<Define wether an m3u file has to be written or not together with the sync operation!*/
  bool SUCCESS;					/*!<Is true if the copy operation is a success. It becomes true just when Progress reach 100%*/
  //******************************************METHOD
  //********************************1st level method
  bool List_all_files();			/*!<List all the file referenced in the playlist file located at *PLAYLIST_PATH and write their address in SONG_PATH_LIST This is an internal function called by Loadplaylist. The function is hence private*/
  bool Define_new_m3u();			/*!<This function create a new m3u file that will be located in *DEVICE_PATH (embeded on the device). The song reference in this m3u will be those contained in *NEW_SONG_PATH. The *NEW_SONG_PATH is obviously modified, so that the path in the playlist is a relative path from *DEVICE_PATH to *NEW_SONG_PATH*/
  bool Autodetect_playlist_type();		/*!<This function autodetect the playlist type (i.e m3u, xspf, wpl etc) based on the extension of the file. The result is stored in PLAYLIST_TYPE IMPORTANT: currently, autodetection of amarok db playlist is not working. The PLAYLIST_TYPE in case of amarok_db has to be forced from elsewhere and this method will then return a true*/
  bool is_Playlist_path_valid();		/*!<This check wether the path is valid i.e: exist, readable. NOT if the file itself is a kind of playlist file supported by this backend or not !*/
  bool is_Device_path_valid();			/*!<This check wether the *DEVICE_PATH exists, is a directory and if we can write into this*/
  bool is_Valid_song_path(QDir song_path);	/*!<This check wether the specified path exists and is a File. Typically used to check if the copy of a given song worked*/
  //*******************2nd level method*************
  bool List_all_files_from_m3u();		/*!<This is usually called by "List_all_files". It will read a m3u files and write all songpath encountered in *SONG_PATH_LIST */
  bool List_all_files_from_xspf();		/*!<This is usually called by "List_all_files". It will read a xspf files and write all songpath encountered in *SONG_PATH_LIST. IMPORTANT: CURRENTLY NOT IMPLEMENTED !!!!!!!!!!!!!!!!!!!!!!*/	
  bool List_all_files_from_wpl();		/*!<This is usually called by "List_all_files". It will read a wpl files and write all songpath encountered in *SONG_PATH_LIST. IMPORTANT: CURRENTLY NOT IMPLEMENTED !!!!!!!!!!!!!!!!!!!!!!*/		
  bool List_all_files_from_amarok_db();		/*!<This is usually called by "List_all_files". It will read a amarok_db playlist and write all songpath encountered in *SONG_PATH_LIST. IMPORTANT: CURRENTLY NOT IMPLEMENTED !!!!!!!!!!!!!!!!!!!!!!*/	
  //********3rd level method************************
  bool Open_local_playlist_file();		/*!<Provides the service of opening a playlist local file (m3u, xspf, wpl ...) and put it contents into *FILE_STREAM. so that the content can be processed in "List_all_files_from_m3u" for example*/
  bool Go_to_the_device_path_position();	/*!<Places the current working directory of the backend into the *DEVICE_PATH directory*/
  bool Go_to_playlist_path_position();		/*!<Places the current working directory of the backend into the *PLAYLIST_PATH directory*/
  bool Go_to_the_song_position(QDir* song_path);/*!<Places the current working directory of the backend into the *song_path directory*/
  QString Get_the_following_record();		/*!<Search the FILE_STREAM for the next record (next path to a song) contains into the stream. Returns empty string ("") if no further record is found. IMPORTANT: CURRENTLY ONLY M3U IMPLEMENTED*/
  QString Get_name_of_file_from_path(QDir* dir);/*!<Return the file name (without extension) for the file located at the *dir path*/
  QDir Build_a_new_path(QDir* device_path, QDir* song_path ); /*!<Build a new path by adding: (the relative path from *PLAYLIST_PATH to *song_path) to (*DEVICE_PATH)*/
  QString Build_a_relative_path_from_here(QDir* device_path, QDir* song_path);/*!<Create a relative path from the *device_path to the *song_path. This function is typically used by "Define_new_m3u" to create an embedded m3u file with relative path to emebede mp3 files*/
  //low level method********************************
  
};

#endif
