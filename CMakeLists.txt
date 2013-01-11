cmake_minimum_required(VERSION 2.6)
enable_language(CXX)
PROJECT(rainyday) 

FIND_PACKAGE(Qt4  REQUIRED)
INCLUDE(${QT_USE_FILE})
SET (QT_USE_QTXMLPATTERNS)
SET (CPB_INCLUDE /home/max/Documents/C/Library/copy_playlist_backend/include)
INCLUDE_DIRECTORIES(${CPB_INCLUDE} ${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR}  ${QT_QTXML_INCLUDE_DIR} ${QT_QTXMLPATTERNS_INCLUDE_DIR} include /home/max/Documents/C/Library/copy_playlist_backend/include) 

### Version number

SET (rainyday_VERSION_MAJOR 0) # The MAJOR version number.
SET (rainyday_VERSION_MINOR 1) # The MINOR version number

### Define variables 



### Directories
#find_library(libcopy-playlist-backend REQUIRED) #find all required librarys (ex here libmy_lib.so would be find and REQUIRED


### Sources
SET(rainyday_SOURCES  src/main.cpp src/mainwindow.cpp) 
SET(rainyday_HEADERS  include/mainwindow.h  ui/ui_mainwindow.h)
SET(rainyday_FORMS  ui/mainwindow.ui)
SET(rainyday_RCCS playlist_ressource.qrc)


### Final part
QT4_WRAP_CPP(rainyday_HEADERS_MOC ${rainyday_HEADERS})
QT4_WRAP_UI(rainyday_FORMS_HEADERS ${rainyday_FORMS})
QT4_ADD_RESOURCES(rainyday_RCC_SRCS ${rainyday_RCCS})
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})
LINK_DIRECTORIES(/home/max/Documents/C/Library/lib/) # Directories for the compiled library (.so and .a) added by command: link_directories(/path/to/,sofiles/)



#IF QT UN-COMMENT THE FOLLOWING LINE
qt4_automoc(${rainyday_SOURCES} ${rainyday_HEADERS_MOC} ${rainyday_RCC_SRCS} ${rainyday_FORMS_HEADERS} ${rainyday_FORMS} )
add_executable(rainyday ${rainyday_SOURCES} ${rainyday_HEADERS_MOC} ${rainyday_RCC_SRCS} ${rainyday_FORMS_HEADERS} ${rainyday_FORMS} ) 
target_link_libraries(rainyday ${QT_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY} ${QT_QTXMLPATTERNS_LIBRARY} /home/max/Documents/C/Library/lib/libcopy_playlist_backend.so)

SET_TARGET_PROPERTIES(
  rainyday
  PROPERTIES
  VERSION 0.9.9)

INSTALL(TARGETS rainyday
  DESTINATION "bin"
      )

INSTALL(FILES images/copy.png
      DESTINATION "share/rainyday"
      )

INSTALL(FILES images/dir.png
      DESTINATION "share/rainyday"
      )

INSTALL(FILES images/playlist.png
      DESTINATION "share/rainyday"
      )

INSTALL(FILES images/rainyday_64.png
      DESTINATION "share/rainyday"
      )

INSTALL(FILES images/rainyday_64.png
      DESTINATION "share/pixmaps/rainyday"
      )