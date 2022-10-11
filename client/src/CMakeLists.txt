set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/logindialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/registrationdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/deletedialog.cpp
)

target_sources(Client PUBLIC ${SOURCE})