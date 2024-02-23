cmake_minimum_required(VERSION 3.0.0)

set(PROJECT eePARS)
project(${PROJECT})
enable_testing()


# Project resources
set(PROJECT_HEADER_PATH "${CMAKE_SOURCE_DIR}/include")
set(EXTERNAL_HEADER_PATH "${CMAKE_SOURCE_DIR}/include/external_projects")
set(INTERNAL_HEADER_PATH "${CMAKE_SOURCE_DIR}/include/internal_projects")
set(TEST_HEADER_PATH "${CMAKE_SOURCE_DIR}/tests")
set(HEADER_PATH ${PROJECT_HEADER_PATH} ${EXTERNAL_HEADER_PATH} ${INTERNAL_HEADER_PATH} ${TEST_HEADER_PATH})

set(MBEDTLS_LIB_PATH "${CMAKE_SOURCE_DIR}/lib/mbedtls")
set(LIBRARY_PATH ${MBEDTLS_LIB_PATH})

set(IPEE_DICTIONARY "${CMAKE_SOURCE_DIR}/src/internal_projects/dictionary.c")
set(IPEE_CONTAINER "${CMAKE_SOURCE_DIR}/src/internal_projects/container.c")
set(INTERNAL_PROJECTS_SOURCES ${IPEE_DICTIONARY} ${IPEE_CONTAINER})

set(CESANTA_SOURCES "${CMAKE_SOURCE_DIR}/src/external_projects/cesanta/mongoose.c" "${CMAKE_SOURCE_DIR}/src/external_projects/cesanta/frozen.c")
set(EXTERNAL_PROJECTS_SOURCES ${CESANTA_SOURCES})

set(APP_SOURCES "${CMAKE_SOURCE_DIR}/src/app/application.c" "${CMAKE_SOURCE_DIR}/src/app/configuration.c")
set(CLIENT_SOURCES "${CMAKE_SOURCE_DIR}/src/client/client.c" "${CMAKE_SOURCE_DIR}/src/client/api.c")
set(SERVER_SOURCES "${CMAKE_SOURCE_DIR}/src/server/server.c" "${CMAKE_SOURCE_DIR}/src/server/api.c")
set(CONTROLLERS_SOURCES "${CMAKE_SOURCE_DIR}/src/server/controllers/authorization.c"
                        "${CMAKE_SOURCE_DIR}/src/server/controllers/welcome.c"
                        "${CMAKE_SOURCE_DIR}/src/server/controllers/server_control.c")
set(LIB_SOURCES ${INTERNAL_PROJECTS_SOURCES} ${EXTERNAL_PROJECTS_SOURCES} ${APP_SOURCES} ${CLIENT_SOURCES} ${SERVER_SOURCES} ${CONTROLLERS_SOURCES})

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