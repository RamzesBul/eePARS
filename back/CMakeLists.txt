cmake_minimum_required(VERSION 3.25.1)

set(PROJECT eePARS)
project(${PROJECT})
add_compile_options(-Wno-discarded-qualifiers -Wno-incompatible-pointer-types -Wno-int-conversion)


# External packages
# Required PostgreSQL 14.
find_package(PostgreSQL REQUIRED)
if (NOT ${PostgreSQL_FOUND})
  message(FATAL_ERROR "PostgreSQL not found!")
endif()

# Project resources: headers
set(PROJECT_HEADER_PATH "${CMAKE_SOURCE_DIR}/include")
set(EXTERNAL_HEADER_PATH "${CMAKE_SOURCE_DIR}/include/external_projects")
set(INTERNAL_HEADER_PATH "${CMAKE_SOURCE_DIR}/include/internal_projects")
set(TEST_HEADER_PATH "${CMAKE_SOURCE_DIR}/tests")
set(HEADER_PATH ${PROJECT_HEADER_PATH}
                ${EXTERNAL_HEADER_PATH}
                ${INTERNAL_HEADER_PATH}
                ${TEST_HEADER_PATH})

# Project resources: sources
set(APP_SOURCES "${CMAKE_SOURCE_DIR}/src/app/application.c" "${CMAKE_SOURCE_DIR}/src/app/configuration.c")
set(CESANTA_SOURCES "${CMAKE_SOURCE_DIR}/src/external_projects/cesanta/mongoose.c" "${CMAKE_SOURCE_DIR}/src/external_projects/cesanta/frozen.c")
set(CLIENT_SOURCES "${CMAKE_SOURCE_DIR}/src/client/client.c" "${CMAKE_SOURCE_DIR}/src/client/api.c")
set(SERVER_SOURCES "${CMAKE_SOURCE_DIR}/src/server/server.c" "${CMAKE_SOURCE_DIR}/src/server/api.c")
set(CONTROLLERS_SOURCES "${CMAKE_SOURCE_DIR}/src/server/controllers/authorization.c"
                        "${CMAKE_SOURCE_DIR}/src/server/controllers/registration.c"
                        "${CMAKE_SOURCE_DIR}/src/server/controllers/welcome.c"
                        "${CMAKE_SOURCE_DIR}/src/server/controllers/server_control.c")
set(DATABASE_SOURCES "${CMAKE_SOURCE_DIR}/src/db/postgres.c" "${CMAKE_SOURCE_DIR}/src/db/db_connection.c")
set(USER_SOURCES "${CMAKE_SOURCE_DIR}/src/user/user.c")
set(LIB_SOURCES ${APP_SOURCES}
                ${CESANTA_SOURCES}
                ${CLIENT_SOURCES}
                ${SERVER_SOURCES}
                ${CONTROLLERS_SOURCES}
                ${DATABASE_SOURCES}
                ${USER_SOURCES})

# Project resources: libraries
set(MBEDTLS_LIB_PATH "${CMAKE_SOURCE_DIR}/lib/mbedtls")
set(IPEE_LIB_PATH "${CMAKE_SOURCE_DIR}/lib/IPee")
set(LIBRARY_PATH  ${MBEDTLS_LIB_PATH} ${IPEE_LIB_PATH})

set (LIB_BINARIES_EXTERNAL "mbedtls" "mbedcrypto" "mbedx509")
set (LIB_BINARIES_INTERNAL "IpEeContainer" "IpEeDictionary" "IpEeThreadpool" "IpEeEvent")
if (WIN32)
  set(LIB_BINARIES  "ws2_32"
                    "bcrypt"
                    ${LIB_BINARIES_EXTERNAL}
                    ${LIB_BINARIES_INTERNAL}
                    PostgreSQL::PostgreSQL)
elseif (UNIX)
  set(LIB_BINARIES  ${LIB_BINARIES_EXTERNAL}
                    ${LIB_BINARIES_INTERNAL}
                    PostgreSQL::PostgreSQL)
endif()


# Project variables
set(PROJECT_LIB ${PROJECT}_lib)
set(PROJECT_APP ${PROJECT}_app)
set(PROJECT_TEST ${PROJECT}_test)


# Copy 'data' folder to build directory
file(COPY "${CMAKE_SOURCE_DIR}/data" DESTINATION "${CMAKE_BINARY_DIR}")


# Subdirectories
add_subdirectory("${CMAKE_SOURCE_DIR}/lib") # Library
add_subdirectory("${CMAKE_SOURCE_DIR}/src") # Application

enable_testing()
add_subdirectory("${CMAKE_SOURCE_DIR}/tests") # Tests