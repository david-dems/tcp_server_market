SET(HANDLER_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/RegisterHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HelloHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OrderHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ActiveOrdersHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BalanceHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SignInHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HistoryHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DealsHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DeleteHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/QuotationsHandler.cpp
)

TARGET_SOURCES(Server PUBLIC ${HANDLER_SOURCE})