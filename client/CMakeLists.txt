cmake_minimum_required(VERSION 3.5)

project(Client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
  ./include 
  ./forms
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(MOC_SOURCE
  ./include/mainwindow.h
  ./include/logindialog.h
  ./include/registrationdialog.h
  ./include/deletedialog.h
)

set(UI_SOURCE
  ./forms/mainwindow.ui
  ./forms/logindialog.ui
  ./forms/registrationdialog.ui
  ./forms/deletedialog.ui
)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

qt5_wrap_cpp(MOC_HEADERS ${MOC_SOURCE})
qt5_wrap_ui(UI_HEADERS ${UI_SOURCE})

add_executable(Client ${MOC_HEADERS} ${UI_HEADERS})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(Client PRIVATE Qt5::Widgets)