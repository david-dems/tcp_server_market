SET(CLIENT_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/client.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ExitProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LoginProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RegistrationProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BalanceProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MakeOrderProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HelloProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ActiveOrdersProcessor.cpp
)

tARGET_SOURCES(client PUBLIC ${CLIENT_SOURCE})