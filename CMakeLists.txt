cmake_minimum_required(VERSION 2.6)
enable_language(CXX)
SET (PROJECT_NAME "rainyday" CACHE STRING "Name of the project")
PROJECT(${PROJECT_NAME}) 
SET(CMAKE_INSTALL_PREFIX "/usr" CACHE INTERNAL "Prefix prepended to install directories" )
# Parameter, to be defined with cmake .. -DBUILD:STRING=WITH_DEVEL_BACKEND
SET(BUILD "RELEASE_BACKEND" CACHE STRING "Some user-specified option") 
# Path for the development version of the backend lib 
SET(DEVEL_LIB "/home/max/Documents/C/Library/lib/" CACHE STRING "Some user-specified option") 
# Path for the header of the devel version of the backend
SET(DEVEL_HEADER "/home/max/Documents/C/Library/include/" CACHE STRING "Some user-specified option") 
### Version number
SET (VERSION_x 1) # The MAJOR version number.
SET (VERSION_y 0) # The MINOR version number
SET (VERSION_z 0) # The BUGFIX version number
FIND_PACKAGE(Qt4  REQUIRED)
INCLUDE(${QT_USE_FILE})
SET (QT_USE_QTXMLPATTERNS)
IF(${BUILD} STREQUAL "RELEASE_BACKEND")
  INCLUDE_DIRECTORIES(/usr/include/ ${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR}  ${QT_QTXML_INCLUDE_DIR} ${QT_QTXMLPATTERNS_INCLUDE_DIR} include ) 
  LINK_DIRECTORIES(/usr/lib/) # Directories for the compiled library (.so and .a) added by command: link_directories(/path/to/,sofiles/)
  MESSAGE(STATUS "**************THIS CONFIGURE RAINYDAY WITH A RELEASE BACKEND")
  #find all required librarys (ex here libmy_lib.so would be find and REQUIRED
  FIND_LIBRARY(CPB_BCKEND copy-playlist-backend REQUIRED) 
ENDIF(${BUILD} STREQUAL "RELEASE_BACKEND")
# By default: we compile with the released version of the copy-playlist backend
IF(${BUILD} STREQUAL "WITH_DEVEL_BACKEND")
  INCLUDE_DIRECTORIES(${DEVEL_HEADER} ${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR}  ${QT_QTXML_INCLUDE_DIR} ${QT_QTXMLPATTERNS_INCLUDE_DIR} include ) 
  LINK_DIRECTORIES(${DEVEL_LIB}) # Directories for the compiled library (.so and .a) added by command: link_directories(/path/to/,sofiles/)
  MESSAGE(STATUS "**************THIS CONFIGURE RAINYDAY WITH A DEVELOPMENT BACKEND")
  #find all required librarys (ex here libmy_lib.so would be find and REQUIRED
  FIND_LIBRARY(CPB_BCKEND copy-playlist-backend ${DEVEL_LIB} REQUIRED) 
ENDIF(${BUILD} STREQUAL "WITH_DEVEL_BACKEND")
### Sources
SET(PROJECT_SRC  	src/main.cpp src/mainwindow.cpp) 
SET(PROJECT_HEAD  	include/mainwindow.h  ui/ui_mainwindow.h)
SET(PROJECT_FORMS  	ui/mainwindow.ui)
SET(PROJECT_RCCS 	playlist_ressource.qrc)
### Final part
QT4_WRAP_CPP(PROJECT_HEAD_MOC ${PROJECT_HEAD})
QT4_WRAP_UI(PROJECT_FORMS_HEADERS ${PROJECT_FORMS})
QT4_ADD_RESOURCES(rainyday_RCC_SRCS ${PROJECT_RCCS})
ADD_DEFINITIONS(${QT_DEFINITIONS})
QT4_AUTOMOC(${PROJECT_SRC} ${PROJECT_HEAD_MOC} ${rainyday_RCC_SRCS} ${PROJECT_FORMS_HEADERS} ${PROJECT_FORMS} )
ADD_EXECUTABLE(${PROJECT_NAME} ${PROJECT_SRC} ${PROJECT_HEAD_MOC} ${rainyday_RCC_SRCS} ${PROJECT_FORMS_HEADERS} ${PROJECT_FORMS} ) 
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${QT_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY} ${QT_QTXMLPATTERNS_LIBRARY} ${CPB_BCKEND})
### Set properties (aka: version number)
SET_TARGET_PROPERTIES(
  ${PROJECT_NAME}
  PROPERTIES
  VERSION ${VERSION_x}.${VERSION_y}.${VERSION_z})
###############################INSTALLATION TARGETS
IF(${BUILD} STREQUAL "RELEASE_BACKEND")
INSTALL(TARGETS ${PROJECT_NAME}
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