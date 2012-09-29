#include<QVector>
#include<QFile>
#include<QString>
#include<QStringList>
#include<QDir>
#include<QFileInfo>
#include<QDebug>
#include<QXmlQuery>
#include<QXmlStreamReader>
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
  DIR_KEEP_ARCH_ROOT=new QString("/home/max/Music");
  FILE_STREAM=0;
  EMBED_M3U=TRUE;
}
copy_playlist_backend::~copy_playlist_backend(){
  delete DEVICE_PATH;
  delete PLAYLIST_PATH;
  delete PLAYLIST_FILE;
  delete LAST_ERROR;
  delete FILE_STREAM;
  int size=SONG_PATH_LIST.size()-1;
  for (int i=size;i>-1;i--)
  {
    SONG_PATH_LIST.remove(i); //TODO ENSURE THER IS NO MEMORY LEAK IN HERE ! (ARE THE DESTRUCTOR OF ALL QDIR WELL CALLED )
  }
    SONG_PATH_LIST.clear();
    SONG_PATH_LIST.squeeze();
  size=NEW_SONG_PATH.size()-1;
  for (int i=size;i>-1;i--)
  {
    NEW_SONG_PATH.remove(i); //TODO ENSURE THER IS NO MEMORY LEAK IN HERE ! (ARE THE DESTRUCTOR OF ALL QDIR WELL CALLED )
  }
    NEW_SONG_PATH.clear();
    NEW_SONG_PATH.squeeze();
    qDebug()<<"end of copy_playlist backend destructor";
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
  if (PLAYLIST_LOADED_FLAG){ //if a playlist is already loaded then update the new path according to the new sync strategy
    Define_new_path();
  }
  return ((synchronization_type==keep_arch)||(synchronization_type==flat)||(synchronization_type==one_folder_per_playlist));
}
void copy_playlist_backend::set_Dir_where_data_struct_kept(QString dir){
  *DIR_KEEP_ARCH_ROOT=dir;
}
void copy_playlist_backend::set_Embed_m3u_file(bool embed){
  EMBED_M3U=embed;
}
QStringList copy_playlist_backend::get_Song_list(){
  QStringList songsname_list;
  if (PLAYLIST_LOADED_FLAG){
    for (int i=0;i<SONG_PATH_LIST.size();i++){   
      songsname_list.append(Get_name_of_file_from_path(& SONG_PATH_LIST[i]));
    }
  }
  return songsname_list;
}
QStringList copy_playlist_backend::get_New_path_list(){
  QStringList dirname_list;
  if (NEW_PATH_UPTODATE_FLAG){
    for (int i=0;i<NEW_SONG_PATH.size();i++){   
      dirname_list.append(NEW_SONG_PATH.at(i).absolutePath());
    }
  }
  return dirname_list;;
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
      name=Get_name_of_file_from_path(PLAYLIST_PATH);	
      }
      //TODO implement geting the playlist name in case of an amarok internal db playlist
    }   
    return name;
}
QString copy_playlist_backend::get_Full_dir_name(){
  return DEVICE_PATH->absolutePath();
}
QString copy_playlist_backend::get_Dir_name(){
  return DEVICE_PATH->dirName();
}
QString copy_playlist_backend::get_Dir_where_data_struct_kept(){
  return *DIR_KEEP_ARCH_ROOT;
}
int copy_playlist_backend::get_Numbers_of_track(){
   return NEW_SONG_PATH.size();
}
int copy_playlist_backend::get_Progress(){
  return PROGRESS;
}
void copy_playlist_backend::Roger_the_notification_of_end_of_operation(){
  qDebug()<<"enter roger the notification of end of operation";
  PROGRESS=0;
  SUCCESS=false;
  emit Progress_changed();
}
bool copy_playlist_backend::Load_playlist(){
  PLAYLIST_LOADED_FLAG=false;
  NEW_PATH_UPTODATE_FLAG=false;
  if (is_Playlist_path_valid()){
    List_all_files();
  }
  if (PLAYLIST_LOADED_FLAG) {
    emit A_new_playlist_is_loaded();
  }
  Define_new_path();   //automatically attempt to define the new path straight after loading them (in case the *DEVICE_PATH is already define)
  return PLAYLIST_LOADED_FLAG;
}
bool copy_playlist_backend::Load_playlist(QString playlist_path){
  set_Playlist_path(playlist_path);
  Load_playlist();
}
bool copy_playlist_backend::Define_new_path(){ 
  NEW_PATH_UPTODATE_FLAG=false;
  NEW_SONG_PATH.clear();
  qDebug()<<"enter define new path";
  if ((is_Device_path_valid()&&PLAYLIST_LOADED_FLAG)){

    if(Go_to_playlist_path_position()){
      qDebug()<<"We are at the playlist position";
      QDir dir_buffer("");
      for (int i=0;i<SONG_PATH_LIST.size();i++){
	dir_buffer=SONG_PATH_LIST.at(i);
	NEW_SONG_PATH.append(Build_a_new_path(DEVICE_PATH, & dir_buffer));
      }
       NEW_PATH_UPTODATE_FLAG=true;     
    }    
  }
  if (NEW_PATH_UPTODATE_FLAG) {
    emit The_dir_is_uptodate();
  }  
  return NEW_PATH_UPTODATE_FLAG;
}
bool copy_playlist_backend::Sync_the_playlist(){
  SUCCESS=false;
  if(PLAYLIST_LOADED_FLAG && NEW_PATH_UPTODATE_FLAG){
    QDir dir_buffer;
    QString path_buff;
    int pos_last_separator;
    for (int i=0;i<NEW_SONG_PATH.size();i++){
      path_buff=NEW_SONG_PATH.at(i).absolutePath();
      pos_last_separator=path_buff.lastIndexOf("/");
      dir_buffer.setPath(path_buff.left(pos_last_separator));
      qDebug()<<"Song nb"<<i<<"gonna be copied";
      qDebug()<<"The song path is"<<SONG_PATH_LIST.at(i).path();
      qDebug()<<"The new path is"<<NEW_SONG_PATH.at(i).path(); 
      qDebug()<<"The dir_buffer is"<<dir_buffer.path();
      qDebug()<<dir_buffer.mkpath(dir_buffer.absolutePath());
      qDebug()<<QFile::copy(SONG_PATH_LIST.at(i).path(),NEW_SONG_PATH.at(i).path());
      PROGRESS=i+1;
      emit Progress_changed();
    }
    if (EMBED_M3U){			//Write a embeded m3u file if needed
      Define_new_m3u();
    }
    SUCCESS=true;			//TODO CHECK THIS !
  }
  qDebug()<<"now emitting the Copy_operation_ended signals";
  emit Copy_operation_ended();
  return SUCCESS;
}
bool copy_playlist_backend::List_all_files(){
  PLAYLIST_LOADED_FLAG=false;
  SONG_PATH_LIST.clear();
  if(Autodetect_playlist_type()){
  switch(PLAYLIST_TYPE){
    case (m3u):{
      PLAYLIST_LOADED_FLAG=List_all_files_from_m3u();
      break;
    }
    case (xspf):{
      PLAYLIST_LOADED_FLAG=List_all_files_from_xspf();
      break;
    }
    case (wpl):{
      PLAYLIST_LOADED_FLAG=List_all_files_from_wpl();
      break;
    }
    case (amarok_db):{
      PLAYLIST_LOADED_FLAG=List_all_files_from_amarok_db();
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
  bool result=true;
  QString path_buff(DEVICE_PATH->path());
  path_buff.append("/");
  path_buff.append(get_Playlist_name());
  path_buff.append(".m3u");
  QDir dir_buffer(path_buff);
  dir_buffer.setPath(QDir::cleanPath(dir_buffer.path()));
  QFile new_m3u(dir_buffer.absolutePath());
  if (new_m3u.open(QFile::WriteOnly | QFile::Truncate)){
    qDebug()<<"the new m3u file is open and created";
    QTextStream streambuffer(& new_m3u);
    streambuffer.readAll(); // TODO Do We need it ?
    streambuffer <<"#EXTM3U";
    endl(streambuffer);
    QDir dir_buffer(*DEVICE_PATH);
    for(int i=0;i<NEW_SONG_PATH.size();i++){
      dir_buffer=NEW_SONG_PATH.at(i);      
      if (Go_to_the_device_path_position()){
	qDebug()<<"We are supposed to be at the song position"<<QDir::currentPath();
      streambuffer<<"#EXTINF";
      endl(streambuffer);
      streambuffer<<Build_a_relative_path_from_here(DEVICE_PATH,&dir_buffer,DEVICE_PATH->absolutePath());
      endl(streambuffer);
      }
      else{
	LAST_ERROR->clear();
	LAST_ERROR->append("Impossible to place the current working dir to:");
	LAST_ERROR->append(dir_buffer.path());
	LAST_ERROR->append("the song won't be part of the playlist");
	emit Error_raised();
    result=false;
      }
    }
    streambuffer.flush();
    new_m3u.close();
  }  
  return result;
  
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
qDebug()<<"The playlist is autodetected as"<<PLAYLIST_TYPE;
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
      if (dir_buffer.isRelative()){
	dir_buffer.setPath(dir_buffer.absolutePath());;
      }      
      if (is_Valid_song_path(dir_buffer)){
	if (!result){
	  result=true; 
	} 		//as soon as we got on valid song we get loaded THIS CHANGE THE PLAYLIST_LOADED_FLAG
	SONG_PATH_LIST.append(dir_buffer);
      }
      else{
	LAST_ERROR->clear();
	LAST_ERROR->append("No file found in:");
	LAST_ERROR->append(dir_buffer.path());
	qDebug()<<"path="<<dir_buffer.path();
	qDebug()<<"current path="<<QDir::currentPath();
	emit Error_raised();
      }
    }
  }
  else{
    LAST_ERROR->clear();
    LAST_ERROR->append("Unable to open the playlist_file:");
    emit Error_raised();
  }
  /*******************************/
  qDebug()<<"end of list_m3u_files, SONG_PATH_LIST ist";
  for(int i=0;i<SONG_PATH_LIST.size();i++){
    qDebug()<<SONG_PATH_LIST.at(i).path();
  }
  qDebug()<<"Result of list_m3u"<<result;
  return result;
}
bool copy_playlist_backend::List_all_files_from_xspf(){
  qDebug()<<"enter the list_all_files_from_xspf function";
  bool result=false;
    if(Open_local_playlist_file()){
    Go_to_playlist_path_position();
    QDir dir_buffer(QDir::current());
    QStringList list_of_locations;
    QXmlStreamReader xmlStream(PLAYLIST_FILE);
    while(!xmlStream.atEnd())
    {
        xmlStream.readNext();
        if(xmlStream.isStartElement())
        {
            // Read the tag name.
            qDebug()<<"name of the xml start element: "<<xmlStream.name().toString();
	    if(xmlStream.name().toString()=="location"){
	      /* ...go to the next. */
	      xmlStream.readNext();
	      /*
	      * This elements needs to contain Characters so we know it's
	      * actually data, if it's not we'll leave.
	      */
	      if(xmlStream.tokenType() != QXmlStreamReader::Characters) {
		  return false; //TODO HANDLE THIS ERROR WHERE THERE IS NOTHING BETWEEN <location> and </location>
	      }
	      list_of_locations.append(xmlStream.text().toString());
	      qDebug()<<"location registered is: "<<list_of_locations.at(list_of_locations.size()-1);
	    }
        }
    }
    //new way
    for(int i=0;i<list_of_locations.size();i++)
    {
      dir_buffer.setPath(list_of_locations.at(i));
      if (dir_buffer.isRelative()){
	dir_buffer.setPath(dir_buffer.absolutePath());
      }
      if (is_Valid_song_path(dir_buffer)){
	if (!result){
	  result=true;
	 } 		//as soon as we got on valid song we get loaded THIS CHANGE THE PLAYLIST_LOADED_FLAG
	 SONG_PATH_LIST.append(dir_buffer);
	}
      else{
	LAST_ERROR->clear();
	LAST_ERROR->append("No file found in:");
	LAST_ERROR->append(dir_buffer.path());
	qDebug()<<"path="<<dir_buffer.path();
	qDebug()<<"current path="<<QDir::currentPath();
	emit Error_raised();
      }
    }
   }    
  else{
    LAST_ERROR->clear();
    LAST_ERROR->append("Unable to open the playlist_file:");
    emit Error_raised();
  }
  /*******************************/
  qDebug()<<"end of list_xspf_files, SONG_PATH_LIST ist";
  for(int i=0;i<SONG_PATH_LIST.size();i++){
    qDebug()<<SONG_PATH_LIST.at(i).path();
  }
  qDebug()<<"Result of list_xspf"<<result;
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
bool copy_playlist_backend::Go_to_the_device_path_position(){
  QDir dir_buffer(*DEVICE_PATH);
  return QDir::setCurrent(dir_buffer.path());;
}
bool copy_playlist_backend::Go_to_playlist_path_position(){
  QDir dir_buffer(*PLAYLIST_PATH);
  dir_buffer.cdUp();
  return QDir::setCurrent(dir_buffer.path());
}
bool copy_playlist_backend::Go_to_the_song_position(QDir* song_path){
  QDir dir_buffer(*song_path);
  dir_buffer.cdUp();
  return QDir::setCurrent(dir_buffer.path());
}
QString copy_playlist_backend::Get_the_following_record(){
 QString line_buf("");
    switch (PLAYLIST_TYPE){
    case(m3u):{
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
  return line_buf;
}
QString copy_playlist_backend::Get_name_of_file_from_path(QDir* dir){
  QString name;
  QFileInfo finfobuffer(*dir,dir->path());
  int pos_of_point=finfobuffer.fileName().lastIndexOf(".");
  name=finfobuffer.fileName().left(pos_of_point);
  return name;
}
QDir copy_playlist_backend::Build_a_new_path(QDir* device_path, QDir* song_path ){
  QDir dir_buffer("");  
  switch(SYNC_TYPE){
    case(keep_arch):{
        Go_to_playlist_path_position();
	QString full_path(Build_a_relative_path_from_here(device_path,song_path,*DIR_KEEP_ARCH_ROOT));
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
QString copy_playlist_backend::Build_a_relative_path_from_here(QDir* device_path,QDir* song_path, QString rootdir){
  QFileInfo finfobuffer(*song_path,song_path->path());
  QString result(finfobuffer.fileName());  
  QDir song_path_buff=*song_path;
  song_path_buff.cdUp();	//places us on the paent dir of the file 
  qDebug()<<" We are creating a relative path from"<<QDir::current() ;
  while ((song_path_buff.absolutePath()!=rootdir)&&(song_path_buff.absolutePath()!=QDir::rootPath())){
    result.prepend("/");
    result.prepend(song_path_buff.dirName());
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
    result.clear();
  }
  return result;
}


