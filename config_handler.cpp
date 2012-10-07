/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  Maxime Haselbauer <maxime.haselbauer@googlemail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include<QVector>
#include<QFile>
#include<QString>
#include<QStringList>
#include<QDir>
#include<QFileInfo>

#include "config_handler.h"

config_handler::config_handler(QString app_name){// TODO create a facultative parameter to this function (the parent path)
  CONF_FILE=0;
  FINFO=new QFileInfo;
  APP_NAME=new QString(app_name);
  APP_NAME->prepend("."); /*!<Prepend a point to the name of the directory to make it a hidden directory*/
  CONFIG_PATH=new QDir;
  CONFIG_PARENT=new QDir;
  
  
  set_Parent_dir(QDir::home());
  set_Application_name(*APP_NAME);
  
  if (is_There_existing_parameter()){
    List_all_parameters(); //automatically attempt to read all parameter if it findes existing parameter files inside the home directory
  }

}
config_handler::~config_handler(){

}
bool config_handler::set_Parent_dir(QString path){
  bool result=false;
  CONFIG_PARENT->setPath(path);
  if (CONFIG_PARENT->isReadable()){ //if the parent directory exists and is readable
    result=true;  
    set_Application_name(*APP_NAME);    
  }  
  return result;
}
bool config_handler::set_Application_name(QString app_name){
  bool result;
  *CONFIG_PATH=*CONFIG_PARENT;
  CONFIG_PATH->cd(*APP_NAME);
  if (CONFIG_PATH->exists("config.conf")){
    CONFIG_PATH->setPath(CONFIG_PATH.absolutePath()<<"config.conf");
    result=true;
  }
    return result;
}
bool config_handler::is_There_existing_parameter(){
  bool result;
  QDir buf_dir=*CONFIG_PARENT;
  if (buf_dir.cd(*APP_NAME)){
    if ((buf_dir.exists("config.conf")){
      result=true;
    }
  }
}
bool config_handler::is_There_existing_parameter(QString path){
  set_Parent_dir();
  return is_There_existing_parameter();
}

bool config_handler::Create_parameter_location(){
  set_Application_name(*APP_NAME); 
  QDir::mkpath(CONFIG_PATH);
  return is_There_existing_parameter();
}




