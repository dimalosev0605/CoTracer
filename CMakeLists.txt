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

include_directories("/home/dima/Documents/Qt_projects/mhc_client")
include_directories(${Boost_INCLUDE_DIR})

link_directories(${Boost_LIBRARY_DIR})

find_package(Qt5 COMPONENTS Core Quick Charts REQUIRED)

if(ANDROID)
    add_library(mhc_client SHARED
      main.cpp
      qml.qrc
      user_validator.cpp
      days_model.cpp
      contacts_model.cpp
      communication_tcp_client.cpp
      base_tcp_client.cpp
      authorization_tcp_client.cpp
      chart_data.cpp
      file_dialog_model.cpp
      path_finder.cpp
      settings_model.cpp
    )
else()
    add_executable(mhc_client
      main.cpp
      qml.qrc
      user_validator.cpp
      days_model.cpp
      contacts_model.cpp
      communication_tcp_client.cpp
      base_tcp_client.cpp
      authorization_tcp_client.cpp
      chart_data.cpp
      file_dialog_model.cpp
      path_finder.cpp
      settings_model.cpp
    )
endif()

target_compile_definitions(mhc_client
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(mhc_client
  PRIVATE Qt5::Core Qt5::Quick Qt5::Charts)
