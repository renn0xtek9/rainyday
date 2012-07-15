#include<QVector>
#include<QFile>
#include<QString>
#include<QStringList>
#include<QDir>
#include<QFileInfo>
#include "copy_playlist_backend.h"


copy_playlist_backend::copy_playlist_backend(){
  SYNC_TYPE=keep_arch;
  PLAYLIST_TYPE=m3u;
  PROGRESS=0;					//Progress of the copy process from 0 to 1
  PLAYLIST_LOADED_FLAG=false;
  NEW_PATH_UPTODATE_FLAG=false;
  DEVICE_PATH=new QDir(QDir::homePath());
  PLAYLIST_PATH=new QDir("");
  LAST_ERROR=new QString("");
  FILE_STREAM=0;
}
copy_playlist_backend::~copy_playlist_backend(){
  delete DEVICE_PATH;
  delete PLAYLIST_PATH;
  delete PLAYLIST_FILE;
}
bool copy_playlist_backend::set_Device_path(QString device_path){
  DEVICE_PATH->setPath(device_path);
  NEW_PATH_UPTODATE_FLAG=false;
  return DEVICE_PATH->isReadable();
}
bool copy_playlist_backend::set_Device_path(QDir device_path){
  DEVICE_PATH->setPath(device_path.absolutePath());
  NEW_PATH_UPTODATE_FLAG=false;
  return DEVICE_PATH->isReadable();
}
bool copy_playlist_backend::set_Playlist_path(QString playlist_path){
  PLAYLIST_PATH->setPath(playlist_path);
  PLAYLIST_LOADED_FLAG=false;
  PLAYLIST_TYPE=unknown;
  return (is_Playlist_path_valid()&&Autodetect_playlist_type());
}
bool copy_playlist_backend::set_Playlist_path(QDir playlist_path){
  PLAYLIST_PATH->setPath(playlist_path.absolutePath());
  PLAYLIST_LOADED_FLAG=false;
  PLAYLIST_TYPE=unknown;
  return (is_Playlist_path_valid()&&Autodetect_playlist_type());
}
bool copy_playlist_backend::set_Sync_type(sync_type synchronization_type){
  SYNC_TYPE=synchronization_type;
  return ((synchronization_type==keep_arch)||(synchronization_type==flat)||(synchronization_type==one_folder_per_playlist));
}
QStringList copy_playlist_backend::get_Song_list(){
  QStringList songsname_list;
  QFileInfo entry_buffer("");
  for (int i=0;i<SONG_PATH_LIST.size();i++){   
    entry_buffer.setFile(SONG_PATH_LIST.at(i).path());
    if(entry_buffer.isFile()){
      songsname_list.append(entry_buffer.fileName());
    }    
  }
}
QString copy_playlist_backend::get_Last_Error(){
  return *LAST_ERROR;
}
QString copy_playlist_backend::get_Playlist_name(){
  QString name("");
  if(is_Playlist_path_valid())
  {
    Autodetect_playlist_type();
    if((PLAYLIST_TYPE==m3u)||(PLAYLIST_TYPE==wpl)||(PLAYLIST_TYPE==xspf)){
      QFileInfo finfobuffer(*PLAYLIST_PATH,PLAYLIST_PATH->path());
      int pos_of_point=finfobuffer.fileName().indexOf(".");
      name=finfobuffer.fileName().left(pos_of_point);	
      }
      //TODO implement geting the playlist name in case of an amarok internal db playlist
    }   
    return name;
}
int copy_playlist_backend::get_Numbers_of_track(){
   return NEW_SONG_PATH.size();
}

int copy_playlist_backend::get_Progress(){
  return PROGRESS;
}
bool copy_playlist_backend::Load_playlist(){
  PLAYLIST_LOADED_FLAG=false;
  NEW_PATH_UPTODATE_FLAG=false;
  if (is_Playlist_path_valid()){
    List_all_files();
  }
  return PLAYLIST_LOADED_FLAG;
}
bool copy_playlist_backend::Load_playlist(QString playlist_path){
  PLAYLIST_LOADED_FLAG=false;
  NEW_PATH_UPTODATE_FLAG=false;
  set_Playlist_path(playlist_path);
    if (is_Playlist_path_valid()){
    List_all_files();
  }
  return PLAYLIST_LOADED_FLAG;  
}
bool copy_playlist_backend::Define_new_path(){  
  if ((is_Device_path_valid()&&PLAYLIST_LOADED_FLAG)){
    NEW_SONG_PATH.clear();
    if(Go_to_playlist_path_position()){
      QDir dir_buffer("");
      for (int i=0;i<SONG_PATH_LIST.size();i++){
	dir_buffer=SONG_PATH_LIST.at(i);
	NEW_SONG_PATH.append(Build_a_new_path(DEVICE_PATH, & dir_buffer));
      }
       NEW_PATH_UPTODATE_FLAG=0;     
    }    
  }
  return NEW_PATH_UPTODATE_FLAG;
}
bool copy_playlist_backend::Sync_the_playlist(){
  bool SUCCESS=false;
  if(PLAYLIST_LOADED_FLAG && NEW_PATH_UPTODATE_FLAG){
    for (int i=0;i<NEW_SONG_PATH.size();i++){
      QFile::copy(SONG_PATH_LIST.at(i).path(),NEW_SONG_PATH.at(i).path());
      PROGRESS=i;
      emit Progress_changed();
    }
    SUCCESS=true;			//TODO CHECK THIS !
  }
  return SUCCESS;
}
bool copy_playlist_backend::List_all_files(){
  PLAYLIST_LOADED_FLAG=false;
  SONG_PATH_LIST.clear();
  if(Autodetect_playlist_type()){
  switch(PLAYLIST_TYPE){
    case (m3u):{
      List_all_files_from_m3u();
      break;
    }
    case (xspf):{
      List_all_files_from_xspf();
      break;
    }
    case (wpl):{
      List_all_files_from_wpl();
      break;
    }
    case (amarok_db):{
      List_all_files_from_amarok_db();
      break;
    }
    case (unknown):{
      LAST_ERROR->clear();
      LAST_ERROR->append("Can't list songs in the playlist: the playlist type is unknown");
      emit Error_raised();
      break;
    }
  }
  }
  return PLAYLIST_LOADED_FLAG;
}
bool copy_playlist_backend::Define_new_m3u(){
  QString path_buff(DEVICE_PATH->path());
  path_buff.append("/");
  path_buff.append(get_Playlist_name());
  path_buff.append("m3u");
  QDir dir_buffer(path_buff);
  dir_buffer.setPath(QDir::cleanPath(dir_buffer.path()));
  QFile new_m3u(dir_buffer.absolutePath());
  if (new_m3u.open(QFile::WriteOnly | QFile::Truncate)){
    QTextStream streambuffer(& new_m3u);
    streambuffer.readAll();
    streambuffer <<"#EXTM3U";
    endl(streambuffer);
    QDir dir_buffer(*DEVICE_PATH);
    for(int i=0;i<NEW_SONG_PATH.size();i++){
      dir_buffer=NEW_SONG_PATH.at(i);
      if (Go_to_the_song_position(&dir_buffer)){
      streambuffer<<"#EXTINF";
      endl(streambuffer);
      streambuffer<<Build_a_relative_path_from_here(DEVICE_PATH,&dir_buffer);
      endl(streambuffer);
      }
      else{
	LAST_ERROR->clear();
	LAST_ERROR->append("Impossible to place the current working dir to:");
	LAST_ERROR->append(dir_buffer.path());
	LAST_ERROR->append("the song won't be part of the playlist");
	emit Error_raised();
      }
    }
    streambuffer.flush();
    new_m3u.close();
  }  
  
  
}
bool copy_playlist_backend::Autodetect_playlist_type(){
  bool autodetected=false;
    if (is_Playlist_path_valid()){
      if (PLAYLIST_PATH->path().endsWith(".m3u")){
	PLAYLIST_TYPE=m3u;
	autodetected=true;
      }
      if (PLAYLIST_PATH->path().endsWith(".xspf")){
	PLAYLIST_TYPE=xspf;
	autodetected=true;
      }
      if (PLAYLIST_PATH->path().endsWith(".wpl")){
	PLAYLIST_TYPE=wpl;
	autodetected=true;
      }
      if (PLAYLIST_TYPE==amarok_db){
	autodetected=true;			// just bypass the detection if its amarok db
      }
  }
if (!autodetected){
  LAST_ERROR->clear();
  LAST_ERROR->append("Playlist type not detected for the path");
  LAST_ERROR->append(PLAYLIST_PATH->path());
  emit Error_raised();
}
return autodetected;
}
bool copy_playlist_backend::is_Playlist_path_valid(){
  QFileInfo finfobuffer(*PLAYLIST_PATH,PLAYLIST_PATH->path());
  bool result=(finfobuffer.exists()&&finfobuffer.isFile()&&finfobuffer.isReadable());
  if (!result){
    if (!finfobuffer.exists()){
      LAST_ERROR->clear();
      LAST_ERROR->append("Error when checking the device path: the device path does not exist");
      emit Error_raised();
    }    
    if (!finfobuffer.isFile()){
      LAST_ERROR->clear();
      LAST_ERROR->append("Error when checking the device path: the path specified is not a File");
      emit Error_raised();
    }
    if(!finfobuffer.isReadable()){
      LAST_ERROR->clear();
      LAST_ERROR->append("Error when checking the device path: the path is not readable. Do you have read permission on this file?");
      emit Error_raised(); 
    }
  }	  
  return  result;
}
bool copy_playlist_backend::is_Device_path_valid(){
  QFileInfo finfobuffer(*DEVICE_PATH,DEVICE_PATH->path());
  bool result=(finfobuffer.exists()&&finfobuffer.isDir()&&finfobuffer.isWritable());
  if (!result){
    if (!finfobuffer.exists()){
      LAST_ERROR->clear();
      LAST_ERROR->append("Error when checking the device path: the device path does not exist");
      emit Error_raised();
    }    
    if (!finfobuffer.isDir()){
      LAST_ERROR->clear();
      LAST_ERROR->append("Error when checking the device path: the path specified is not a directory");
      emit Error_raised();
    }
    if (!finfobuffer.isWritable()){
      LAST_ERROR->clear();
      LAST_ERROR->append("Error when checking the device path: the path is not writable. Do you have write permission on this device?");
      emit Error_raised(); 
    }
  }	  
  return  result;
}
bool copy_playlist_backend::is_Valid_song_path(QDir song_path){
  QFileInfo finfobuffer(song_path,song_path.path());
  return (finfobuffer.exists()&&finfobuffer.isFile());
}
bool copy_playlist_backend::List_all_files_from_m3u(){
  bool result=false;
  if(Open_local_playlist_file()){
    Go_to_playlist_path_position();
    QDir dir_buffer(QDir::current());
    while(FILE_STREAM->atEnd()==FALSE){
      dir_buffer.setPath(Get_the_following_record());
      if (is_Valid_song_path(dir_buffer)){
	SONG_PATH_LIST.append(dir_buffer);
      }
      else{
	LAST_ERROR->clear();
	LAST_ERROR->append("No file found in:");
	LAST_ERROR->append(dir_buffer.path());
	emit Error_raised();
      }
    }
  }
  else{
    LAST_ERROR->clear();
    LAST_ERROR->append("Unable to open the playlist_file:");
    emit Error_raised();
  }
  return result;
}
bool copy_playlist_backend::List_all_files_from_xspf(){
  bool result=false;
  return result;
}
bool copy_playlist_backend::List_all_files_from_wpl(){
  bool result=false;
  return result;
}
bool copy_playlist_backend::List_all_files_from_amarok_db(){
  bool result=false;
  return result;
}



bool copy_playlist_backend::Open_local_playlist_file(){
  QFile* Filebuffer=new QFile(PLAYLIST_PATH->path());
  Filebuffer->open(QIODevice::ReadOnly);
  if (Filebuffer->isOpen()){
    PLAYLIST_FILE=Filebuffer;		//WARNING need to ensure that when the function close the QFile pointed by Flbuffer won't be destroyed
    QTextStream* streambuffer=new QTextStream(PLAYLIST_FILE);
    FILE_STREAM=streambuffer;		//WARNING same thing here
  }
  return Filebuffer->isOpen();
}
bool copy_playlist_backend::Go_to_playlist_path_position(){
  QDir dir_buffer(*PLAYLIST_PATH);
  dir_buffer.cdUp();
  return QDir::setCurrent(dir_buffer.path());
}
bool copy_playlist_backend::Go_to_the_song_position(QDir* song_path)
{
  QDir dir_buffer(*song_path);
  dir_buffer.cdUp();
  return QDir::setCurrent(dir_buffer.path());
}

QString copy_playlist_backend::Get_the_following_record(){
  switch (PLAYLIST_TYPE){
    case(m3u):{
      QString line_buf(""); 
      bool record_flag=false;
      while ((!record_flag)&&(FILE_STREAM->atEnd()==FALSE))             // as long as we dont reach the end of the playlist file
	    {
		line_buf=FILE_STREAM->readLine(2048);           // reading the next line
		if (line_buf.startsWith("#EXTINF"))        // the 7 first char ARE "#EXTINF" (wich mean we got a new entry)
		{
		  record_flag=true; //we have found a new entry
		  line_buf=FILE_STREAM->readLine(2048); //the path is located on the line under #EXTINF
		}
	    }
	    return line_buf;
      break;
    }
    case(xspf):{
      ;//TODO implement how to get a record from a xspf
      break;
    }
    case(wpl):{
      ;//TODO implement how to get a record from a wpl
      break;
    }
    case(amarok_db):{
      ;//TODO implement how to get a next track from an amarok internal db playlist
      break;
    }
   }
}


QDir copy_playlist_backend::Build_a_new_path(QDir* device_path, QDir* song_path ){
  QDir dir_buffer("");  
  switch(SYNC_TYPE){
    case(keep_arch):{
        Go_to_playlist_path_position();
	QString full_path(Build_a_relative_path_from_here(device_path,song_path));
	if (!full_path.isEmpty()){
	  full_path.prepend("/");
	  full_path.prepend(device_path->absolutePath());
	  dir_buffer.setPath(full_path);
	}
      break;
    }
    case(flat):{
      QFileInfo finfobuffer(*song_path,song_path->path());
      QString path_buff(device_path->absolutePath());
      path_buff.append("/");
      path_buff.append(finfobuffer.fileName());
      dir_buffer.setPath(path_buff);
      break;
    }
    case(one_folder_per_playlist):{
      QFileInfo finfobuffer(*song_path,song_path->path());
      QString path_buff(device_path->absolutePath());
      path_buff.append("/");
      path_buff.append(get_Playlist_name());
      path_buff.append("/");
      path_buff.append(finfobuffer.fileName());
      dir_buffer.setPath(path_buff);
      break;
    }
  }
  return dir_buffer;  
}
QString copy_playlist_backend::Build_a_relative_path_from_here(QDir* device_path,QDir* song_path){
	QString result;
	QFileInfo finfobuffer(*song_path,song_path->path());
	QStringList end_of_path(finfobuffer.fileName());  
	QDir song_path_buff=*song_path;
	song_path_buff.cdUp();	//places us on the paent dir of the file 
	while ((song_path_buff!=QDir::current())&&(song_path_buff.absolutePath()!=QDir::rootPath())){
	  end_of_path.prepend("/");
	  end_of_path.prepend(song_path_buff.dirName());
	  song_path_buff.cdUp();
	}
	if ((song_path_buff.path()==QDir::rootPath())&&(song_path_buff!=QDir::current())){
	  LAST_ERROR->clear();
	  LAST_ERROR->append("Error when a relative path for");
	  LAST_ERROR->append(song_path->path());
	  LAST_ERROR->append("from:");
	  LAST_ERROR->append(QDir::currentPath());
	  LAST_ERROR->append("the root path has been reached without meeting the current path");
	  emit Error_raised();
	  QString.clear();
	}
	else{
	  for (int i=0;i<end_of_path.size();i++){
	    result.append(end_of_path.at(i));
	  }
	}
	return result;
}



