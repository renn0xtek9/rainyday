cmake_minimum_required(VERSION 2.6)
enable_language(CXX)
PROJECT(QPlaylisTransfer) # This give the project a name

#IF QT UN-COMMENT THE FOLLOWING LINE
FIND_PACKAGE(Qt4 REQUIRED)

### Version number
SET (project_name_VERSION_MAJOR 1) # The MAJOR version number.
SET (project_name_VERSION_MINOR 0) # The MINOR version number

### Define variables 
#set(CMAKE_CXX_COMPILER GCC) # <variable> can be an environment variable such as: set( ENV{PATH} /home/martink )

### Directories
#find_library(my_lib REQUIRED) #find all required librarys (ex here libmy_lib.so would be find and REQUIRED
INCLUDE_DIRECTORIES(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ) #Directories for the  header files (.h)


### Sources
SET(QPlaylisTransfer_SOURCES copy_playlist_backend.cpp main.cpp mainwindow.cpp) # collect the sources of the project and call the collected set  "project_name_SRCS"
SET(QPlaylisTransfer_HEADERS copy_playlist_backend.h mainwindow.h ui_mainwindow.h)
SET(QPlaylisTransfer_FORMS mainwindow.ui)
SET(QPlaylisTransfer_RESSOURCES)
### Final part
QT4_WRAP_CPP(QPlaylisTransfer_HEADERS_MOC ${QPlaylisTransfer_HEADERS})
QT4_WRAP_UI(QPlaylisTransfer_FORMS_HEADERS ${QPlaylisTransfer_FORMS})
QT4_ADD_RESOURCES(QPlaylisTransfer_RESOURCES_RCC ${QPlaylisTransfer_RESOURCES})
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})
LINK_DIRECTORIES(/home/max/Documents/C/Library/lib/) # Directories for the compiled library (.so and .a) added by command: link_directories(/path/to/,sofiles/)



#IF QT UN-COMMENT THE FOLLOWING LINE
qt4_automoc(${QPlaylisTransfer_SOURCES}) #don't know what it does but I think it is kind of better when it is there
add_executable(QPlaylisTransfer ${QPlaylisTransfer_SOURCES} ${QPlaylisTransfer_HEADERS_MOC}) #tell cmake to create the executable "project_nam" based on the sources collected in "project_name_SRCS
target_link_libraries(QPlaylisTransfer ${QT_LIBRARIES}) #link the program with the library with target_link_libraries(/path/to/lib.so)
#Do always give the full path for all of your .so files