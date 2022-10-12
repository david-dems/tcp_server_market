CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(ClienServerEcn)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

FIND_PACKAGE(Boost 1.40 COMPONENTS system REQUIRED)
FIND_PACKAGE(Threads REQUIRED)
FIND_PACKAGE(PostgreSQL REQUIRED)

INCLUDE_DIRECTORIES(
    ${Boost_INCLUDE_DIR}
    ${PostgreSQL_INCLUDE_DIRS}
    ./include 
    ./include/postgres
    ./include/handlers
    ./include/client
    ./include/matchengine
    ./googletest/include
    )

SET(CMAKE_CXX_STANDARD 17)

ADD_EXECUTABLE(Server "")
ADD_EXECUTABLE(client "")
ADD_EXECUTABLE(run_tests "")

ADD_SUBDIRECTORY(./client)
ADD_SUBDIRECTORY(./src)
ADD_SUBDIRECTORY(./src/postgres)
ADD_SUBDIRECTORY(./src/handlers)
ADD_SUBDIRECTORY(./src/client)
ADD_SUBDIRECTORY(./src/matchengine)
ADD_SUBDIRECTORY(./tests)

TARGET_LINK_LIBRARIES(Server PRIVATE Threads::Threads ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(Server PRIVATE ${PostgreSQL_LIBRARIES})

TARGET_LINK_LIBRARIES(client PRIVATE Threads::Threads ${Boost_LIBRARIES})

SET(gtest_force_shared_crt ON)
SET(INSTALL_GTEST OFF)
SET(BUILD_GMOCK OFF)
ADD_SUBDIRECTORY(./googletest)
TARGET_LINK_LIBRARIES(run_tests PRIVATE Threads::Threads ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(run_tests PRIVATE ${PostgreSQL_LIBRARIES})
TARGET_LINK_LIBRARIES(run_tests PRIVATE gtest gtest_main)
