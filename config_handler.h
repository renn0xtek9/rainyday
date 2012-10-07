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

/**
 * \struct config_handler
 * \brief this class is used to handle config parameter of a QT programm. It will save parameters in the home directory.
 * or in a given directory. The parameter are then typically read at the ignition of the ui to recover previously set parameter
 * of the ui (e.g size, preselcted directory etc ...) *  
 */

#ifndef CONFIG_HANDLER_H
#define CONFIG_HANDLER_H

#include<QDir>
#include<QVector>
#include<QFile>
#include<QString>
#include<QStringList>
#include <QTextStream>

/*!<*/

class config_handler
{

public:
    config_handler(QString app_name);
    virtual ~config_handler();
  //******************************************SETORS
    bool set_Application_name(QString app_name); /*!<Define the name of the application. This name will be prefixed with a . and a directory will be created with this name in the home directory*/
    bool set_Parent_dir(QString path); /*!<Define a directory that is paraent for the directory which will contains the parameter. By default it is $HOME dir. Do only use this function if you want to specify an other directory. You will need to have write access to this directory. If no parameter directory exist in this path, it will try to create one, if it does not succeed it returns zero*/
    bool set_Value_of_param(QString name_of_param, QString value_of_param);/*!<Record a parameter by its name and its value*/ 
  //******************************************ACCESSORS      
    bool is_There_existing_parameter();/*!<Return one if the functions find an already existing parameter_file in the given directory. Return zero if no or if it can not open the file*/
    bool is_There_existing_parameter(QString path); /*!<Overloaded function to define the parent directory and then search for existing parameter*/
    QString get_Last_error(); /*!<Returns the last error message*/
    QString get_Value_of_param(QString name_of_param);/*!< Get the value of a given parameter. Empty if does not exist or if no config_file is accessible*/
 //******************************************ACCESSORS


    
private:
  
  QDir *CONFIG_PATH;/*!<Full path of the config file. E.g (/home/kim_jong_il/.rainyday/config.conf) */
  QDir *CONFIG_PARENT;/*!<Full path of the parent directory of the config file. E.g (/home/kim_jong_il/) */
  QString *LAST_ERROR;/*!<Contains the Last error messge*/
  QString *APP_NAME;/*!<Containts the name (not the full path) of the directory in which the parameter file is located. Typically the name of application prefixed with a point (Eg ".rainyday"*/
  QFile *CONF_FILE;/*!<Config file itself*/
  QFileInfo *FINFO;/*!<Informations on the QFile */
  QVector<QString> PARAMETERS; /*!< List of all parameters*/

  
  //******************************************PRIVATE ACTIONS
  bool Create_parameter_location();/*!<The function that attempt to create parameter locations (i.e the directory in the $HOME dir plus the file inside). Return one if it succeeds*/
  bool List_all_parameters();/*!<List all parameters and put them to a List*/ 
  
};

#endif // CONFIG_HANDLER_H
