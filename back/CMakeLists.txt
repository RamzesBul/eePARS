cmake_minimum_required(VERSION 3.0.0)

set(PROJECT eePARS)
project(${PROJECT})
enable_testing()


# Resources
set(PROJECT_HEADER_PATH "${CMAKE_SOURCE_DIR}/include")
set(EXTERNAL_HEADER_PATH "${CMAKE_SOURCE_DIR}/include/external_projects")
set(TEST_HEADER_PATH "${CMAKE_SOURCE_DIR}/tests")
set(HEADER_PATH ${PROJECT_HEADER_PATH} ${EXTERNAL_HEADER_PATH} ${TEST_HEADER_PATH})

set(MBEDTLS_LIB_PATH "${CMAKE_SOURCE_DIR}/lib/mbedtls")
set(EXTERNAL_LIB_PATH ${MBEDTLS_LIB_PATH})
set(LIBRARY_PATH ${EXTERNAL_LIB_PATH})

set(CORE_SOURCES "${CMAKE_SOURCE_DIR}/src/core/configuration.c")
set(APP_SOURCES "${CMAKE_SOURCE_DIR}/src/app/application.c" "${CMAKE_SOURCE_DIR}/src/app/service.c")
set(SERVER_SOURCES "${CMAKE_SOURCE_DIR}/src/server/server.c" "${CMAKE_SOURCE_DIR}/src/server/webapi/api.c")
set(CONTROLLERS_SOURCES "${CMAKE_SOURCE_DIR}/src/server/controllers/authorization.c" "${CMAKE_SOURCE_DIR}/src/server/controllers/welcome.c")
set(CLIENT_SOURCES "${CMAKE_SOURCE_DIR}/src/client/client.c" "${CMAKE_SOURCE_DIR}/src/client/api.c")
set(CESANTA_SOURCES "${CMAKE_SOURCE_DIR}/src/external_projects/cesanta/mongoose.c" "${CMAKE_SOURCE_DIR}/src/external_projects/cesanta/frozen.c")
set(LIB_SOURCES ${CORE_SOURCES} ${APP_SOURCES} ${SERVER_SOURCES} ${CONTROLLERS_SOURCES} ${CLIENT_SOURCES} ${CESANTA_SOURCES})

if (WIN32)
  set(LIB_BINARIES "ws2_32" "mbedtls" "mbedcrypto" "mbedx509" "bcrypt")
elseif (UNIX)
  set(LIB_BINARIES "mbedtls" "mbedcrypto" "mbedx509")
endif()


# Project variables
set(PROJECT_LIB ${PROJECT}_lib)
set(PROJECT_APP ${PROJECT}_app)
set(WEBAPI_TEST ${PROJECT}_webapi_test)


# Copy 'data' folder to build directory
file(COPY "${CMAKE_SOURCE_DIR}/data" DESTINATION "${CMAKE_BINARY_DIR}")


# Subdirectories
add_subdirectory("${CMAKE_SOURCE_DIR}/lib") # Library
add_subdirectory("${CMAKE_SOURCE_DIR}/src") # Application
add_subdirectory("${CMAKE_SOURCE_DIR}/tests") # Tests