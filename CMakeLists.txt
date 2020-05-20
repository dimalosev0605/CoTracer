cmake_minimum_required(VERSION 3.5)

project(mhc_client LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Boost_INCLUDE_DIR /home/dima/Boost/boost_1_72_0/Boost_Library_Binary/include)
set(Boost_LIBRARY_DIR /home/dima/Boost/boost_1_72_0/Boost_Library_Binary/lib)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

include_directories("home/dima/Documents/Qt_projects/mhc_client/networking/headers")
include_directories("home/dima/Documents/Qt_projects/mhc_client/models/headers")
include_directories("home/dima/Documents/Qt_projects/mhc_client/miscallaneous")
include_directories(${Boost_INCLUDE_DIR})

link_directories(${Boost_LIBRARY_DIR})

find_package(Qt5 COMPONENTS Core Quick Charts REQUIRED)

if(ANDROID)
    add_library(mhc_client SHARED
      main.cpp
      qml.qrc
      ${CMAKE_CURRENT_SOURCE_DIR}/models/sources/days_model.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/models/sources/contacts_model.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/models/sources/chart_data.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/models/sources/file_dialog_model.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/models/sources/settings_model.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/networking/sources/base_tcp_client.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/networking/sources/communication_tcp_client.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/networking/sources/authorization_tcp_client.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/miscellaneous/sources/user_validator.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/miscellaneous/sources/path_finder.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/miscellaneous/sources/fad_manager.cpp
    )
else()
    add_executable(
      mhc_client
      main.cpp
      qml.qrc

      #####
      ${CMAKE_CURRENT_SOURCE_DIR}/models/headers/days_model.h
      ${CMAKE_CURRENT_SOURCE_DIR}/models/headers/contacts_model.h
      ${CMAKE_CURRENT_SOURCE_DIR}/models/headers/chart_data.h
      ${CMAKE_CURRENT_SOURCE_DIR}/models/headers/file_dialog_model.h
      ${CMAKE_CURRENT_SOURCE_DIR}/models/headers/settings_model.h
      #---
      ${CMAKE_CURRENT_SOURCE_DIR}/models/sources/days_model.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/models/sources/contacts_model.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/models/sources/chart_data.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/models/sources/file_dialog_model.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/models/sources/settings_model.cpp
      #####

      #####
      ${CMAKE_CURRENT_SOURCE_DIR}/networking/headers/base_tcp_client.h
      ${CMAKE_CURRENT_SOURCE_DIR}/networking/headers/communication_tcp_client.h
      ${CMAKE_CURRENT_SOURCE_DIR}/networking/headers/authorization_tcp_client.h
      #---
      ${CMAKE_CURRENT_SOURCE_DIR}/networking/sources/base_tcp_client.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/networking/sources/communication_tcp_client.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/networking/sources/authorization_tcp_client.cpp
      #####

      #####
      ${CMAKE_CURRENT_SOURCE_DIR}/miscellaneous/headers/user_validator.h
      ${CMAKE_CURRENT_SOURCE_DIR}/miscellaneous/headers/path_finder.h
      ${CMAKE_CURRENT_SOURCE_DIR}/miscellaneous/headers/fad_manager.h
      #---
      ${CMAKE_CURRENT_SOURCE_DIR}/miscellaneous/sources/user_validator.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/miscellaneous/sources/path_finder.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/miscellaneous/sources/fad_manager.cpp
      #####
    )
endif()

target_compile_definitions(mhc_client
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(mhc_client
  PRIVATE Qt5::Core Qt5::Quick Qt5::Charts)
