cmake_minimum_required(VERSION 2.6)
enable_language(CXX)
PROJECT(Rainyday) # This give the project a name

#IF QT UN-COMMENT THE FOLLOWING LINE
FIND_PACKAGE(Qt4  REQUIRED)
### Other qt modules
SET (QT_USE_QTXMLPATTERNS)


### Version number

SET (project_name_VERSION_MAJOR 0) # The MAJOR version number.
SET (project_name_VERSION_MINOR 1) # The MINOR version number

### Define variables 
#set(CMAKE_CXX_COMPILER GCC) # <variable> can be an environment variable such as: set( ENV{PATH} /home/martink )
SET (CPB_INCLUDE /home/max/Documents/C/Library/copy_playlist_backend/include)

### Directories
#find_library(my_lib REQUIRED) #find all required librarys (ex here libmy_lib.so would be find and REQUIRED
INCLUDE_DIRECTORIES(${CPB_INCLUDE} ${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR}  ${QT_QTXML_INCLUDE_DIR} ${QT_QTXMLPATTERNS_INCLUDE_DIR}) #Directories for the  header files (.h)

### Sources
SET(Rainyday_SOURCES  main.cpp mainwindow.cpp) # collect the sources of the project and call the collected set  "project_name_SRCS"
SET(Rainyday_HEADERS  mainwindow.h  ui_mainwindow.h)
SET(Rainyday_FORMS  mainwindow.ui)
SET(Rainyday_RCCS playlist_ressource.qrc)
### Final part
QT4_WRAP_CPP(Rainyday_HEADERS_MOC ${Rainyday_HEADERS})
QT4_WRAP_UI(Rainyday_FORMS_HEADERS ${Rainyday_FORMS})
QT4_ADD_RESOURCES(Rainyday_RCC_SRCS ${Rainyday_RCCS})
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})
LINK_DIRECTORIES(/home/max/Documents/C/Library/lib/) # Directories for the compiled library (.so and .a) added by command: link_directories(/path/to/,sofiles/)



#IF QT UN-COMMENT THE FOLLOWING LINE
qt4_automoc(${Rainyday_SOURCES} ${Rainyday_HEADERS_MOC} ${Rainyday_RCC_SRCS} ${Rainyday_FORMS_HEADERS} ${Rainyday_FORMS} ) #don't know what it does but I think it is kind of better when it is there
add_executable(Rainyday ${Rainyday_SOURCES} ${Rainyday_HEADERS_MOC} ${Rainyday_RCC_SRCS} ${Rainyday_FORMS_HEADERS} ${Rainyday_FORMS} ) #tell cmake to create the executable "project_nam" based on the sources collected in "project_name_SRCS
target_link_libraries(Rainyday ${QT_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY} ${QT_QTXMLPATTERNS_LIBRARY} /home/max/Documents/C/Library/lib/libcopy_playlist_backend.so) #link the program with the library with target_link_libraries(/path/to/lib.so)
#Do always give the full path for all of your .so files