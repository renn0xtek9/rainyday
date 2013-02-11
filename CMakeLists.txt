cmake_minimum_required(VERSION 2.6)
enable_language(CXX)
PROJECT(rainyday) 

SET(BUILD "WITH_DEVEL_BACKEND" CACHE STRING "Some user-specified option") # Parameter, to be defined with cmake .. -DBUILD:STRING=WITH_DEVEL_BACKEND
SET(PATH_DEVEL_BACKEND_LIB "/home/max/Documents/C/Library/lib/" CACHE STRING "Some user-specified option") # Path for the development version of the backend lib 
SET(PATH_DEVEL_BACKEND_HEAD "/home/max/Documents/C/Library/copy_playlist_backend/include/" CACHE STRING "Some user-specified option") # Path for the header of the devel version of the backend


### Version number

SET (rainyday_VERSION_x 0) # The MAJOR version number.
SET (rainyday_VERSION_y 9) # The MINOR version number
SET (rainyday_VERSION_z 9) 


FIND_PACKAGE(Qt4  REQUIRED)
INCLUDE(${QT_USE_FILE})
SET (QT_USE_QTXMLPATTERNS)
IF(${BUILD} STREQUAL "RELEASE_BACKEND")
  SET (CPB_INCLUDE /usr/include/)
  INCLUDE_DIRECTORIES(${CPB_INCLUDE} ${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR}  ${QT_QTXML_INCLUDE_DIR} ${QT_QTXMLPATTERNS_INCLUDE_DIR} include ) 
  LINK_DIRECTORIES(/usr/lib/) # Directories for the compiled library (.so and .a) added by command: link_directories(/path/to/,sofiles/)
  MESSAGE(STATUS "**************THIS CONFIGURE RAINYDAY WITH A RELEASE BACKEND")
ENDIF(${BUILD} STREQUAL "RELEASE_BACKEND")

# By default: we compile with the latest, developemt version of the copy-playlist backend
IF(${BUILD} STREQUAL "WITH_DEVEL_BACKEND")
  INCLUDE_DIRECTORIES(${PATH_DEVEL_BACKEND_HEAD} ${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR}  ${QT_QTXML_INCLUDE_DIR} ${QT_QTXMLPATTERNS_INCLUDE_DIR} include ) 
  LINK_DIRECTORIES(${PATH_DEVEL_BACKEND_LIB}) # Directories for the compiled library (.so and .a) added by command: link_directories(/path/to/,sofiles/)
  MESSAGE(STATUS "**************THIS CONFIGURE RAINYDAY WITH A DEVELOPMENT BACKEND")
ENDIF(${BUILD} STREQUAL "WITH_DEVEL_BACKEND")

### Directories
FIND_LIBRARY(libcopy_playlist_backend ${PATH_DEVEL_BACKEND_LIB} REQUIRED) #find all required librarys (ex here libmy_lib.so would be find and REQUIRED
MESSAGE (STATUS "${PATH_DEVEL_BACKEND_LIB}")

### Sources
SET(rainyday_SOURCES  	src/main.cpp src/mainwindow.cpp) 
SET(rainyday_HEADERS  	include/mainwindow.h  ui/ui_mainwindow.h)
SET(rainyday_FORMS  	ui/mainwindow.ui)
SET(rainyday_RCCS 	playlist_ressource.qrc)


### Final part
QT4_WRAP_CPP(rainyday_HEADERS_MOC ${rainyday_HEADERS})
QT4_WRAP_UI(rainyday_FORMS_HEADERS ${rainyday_FORMS})
QT4_ADD_RESOURCES(rainyday_RCC_SRCS ${rainyday_RCCS})
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})



QT4_AUTOMOC(${rainyday_SOURCES} ${rainyday_HEADERS_MOC} ${rainyday_RCC_SRCS} ${rainyday_FORMS_HEADERS} ${rainyday_FORMS} )
ADD_EXECUTABLE(rainyday ${rainyday_SOURCES} ${rainyday_HEADERS_MOC} ${rainyday_RCC_SRCS} ${rainyday_FORMS_HEADERS} ${rainyday_FORMS} ) 
TARGET_LINK_LIBRARIES(rainyday ${QT_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY} ${QT_QTXMLPATTERNS_LIBRARY} /home/max/Documents/C/Library/lib/libcopy_playlist_backend.so)


SET_TARGET_PROPERTIES(
  rainyday
  PROPERTIES
  VERSION ${rainyday_VERSION_x}.${rainyday_VERSION_y}.${rainyday_VERSION_z})

###############################INSTALLATION TARGETS
IF(${BUILD} STREQUAL "RELEASE_BACKEND")
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
ENDIF(${BUILD} STREQUAL "RELEASE_BACKEND")