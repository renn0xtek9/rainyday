#!/bin/bash
echo -e "\n\nWelcome in this interactive script that helps you compile Rainyday"
echo -e "Do you want to compile rainyday with a development backend version (in one of your directory) or with a realased backend ? (d/r)"
read buildversion
if [ $buildversion == "d" ] ;then
    echo -e "Default location of backend header files: \n$CPB_DEV_HEADER\n\nDefault location of backend lib (.so) files:\n$CPB_DEV_LIB\n\nDo you need to change this and use custom path ? yes/no "
    read answer
	if [ $answer == "yes" ] ;then
	  echo -e "Location of the header file of the devlopment version of copy_playlist_backend: \nDefault is $CPB_DEV_HEADER ?"
	  read header_location 
	  echo -e "Location of the lib (.so) file of the devlopment version of copy_playlist_backend: \nDefault is $CPB_DEV_LIB ?"
	  read lib_location
	  export CPB_DEV_HEADER=$header_location
	  export CPB_DEV_LIB=$lib_location
	fi
 mkdir -p build 
 cd build 
 rm -r -f *
 cmake .. -DBUILD:STRING=WITH_DEVEL_BACKEND -DPATH_DEVEL_BACKEND_HEAD:STRING=$header_location -DPATH_DEVEL_BACKEND_LIB:STRING=$lib_location
 make 
fi 
if [ $buildversion == "r" ] ;then
  mkdir -p build 
  cd build 
  rm -r -f *
  cmake .. -DBUILD:STRING=RELEASE_BACKEND
  make
fi 

echo "You now need to cd into build and make install (if the make operation was successful)"