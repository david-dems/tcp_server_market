SET(CLIENT_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/client.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ExitProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LoginProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RegistrationProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BalanceProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MakeOrderProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HelloProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ActiveOrdersProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HistoryProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DealsProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DeleteProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/QuotationProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/EventProcessorFactory.cpp
)

SET(GUI_CLIENT_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/ExitProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LoginProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RegistrationProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BalanceProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MakeOrderProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HelloProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ActiveOrdersProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HistoryProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DealsProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DeleteProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/QuotationProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/EventProcessorFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ForGuiLoginProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ForGuiDeleteProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ForGuiOrderProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ForGuiRegistrationProcessor.cpp

)

TARGET_SOURCES(CLI_client PUBLIC ${CLIENT_SOURCE})
TARGET_SOURCES(GUI_client PUBLIC ${GUI_CLIENT_SOURCE})