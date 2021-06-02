cmake_minimum_required (VERSION 2.6 FATAL_ERROR)

set_property(GLOBAL PROPERTY USE_FOLDERS On)
project      (test_qipedit)
find_package (Qt5Widgets)

set(CMAKE_AUTORCC ON)

set  (project_SOURCES
    main.cpp
    ip_edit.cpp
)

set  (INCLUDE_FILE
    ip_edit.h
)

set  (QT_UI_HEADERS
       ip_edit.h
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

set  (project_HEADERS ${INCLUDE_FILE} ${QT_UI_HEADERS})

QT5_WRAP_CPP (project_HEADERS_MOC  ${QT_UI_HEADERS})

ADD_DEFINITIONS (${QT_DEFINITIONS})
ADD_DEFINITIONS(-D _XKEYCHECK_H)

ADD_EXECUTABLE  (ip_edit
                                ${project_SOURCES}
                                ${project_HEADERS_MOC}
                                )

#/SUBSYSTEM:WINDOWS and /ENTRY:mainCRTStartup need to config same time
Set_Target_Properties(ip_edit PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

#set vs startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ip_edit)


IF (WIN32)
    TARGET_LINK_LIBRARIES (ip_edit  ${QT_LIBRARIES} Qt5::Widgets)
ELSE (WIN32)
    TARGET_LINK_LIBRARIES (ip_edit ${QT_LIBRARIES})
ENDIF (WIN32)
