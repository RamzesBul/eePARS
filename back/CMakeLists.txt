# CMake configuration
cmake_minimum_required(VERSION 3.0.0)

project(eePARS VERSION 0.1.0)

# Script variables
set(CESANTA_SOURCES "src/cesanta/mongoose.c" "src/cesanta/frozen.c")
set(CORE_SOURCES "src/core/client.c" "src/core/server.c" "src/core/service.c" "src/core/cred.c")
set(SOURCES "main.c" ${CESANTA_SOURCES} ${CORE_SOURCES})

# Script properties
add_executable(${PROJECT_NAME} ${SOURCES})

# Preprocessor flags
target_compile_definitions(${PROJECT_NAME} PUBLIC -DMG_ENABLE_MBEDTLS=1)

# Compile flags
target_compile_options(${PROJECT_NAME} PUBLIC -Wdiscarded-qualifiers)

# Headers directory
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")

# Libraries directory
target_link_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/lib" "${PROJECT_SOURCE_DIR}/lib/mbedtls")

# Link libraries
target_link_libraries(${PROJECT_NAME} "ws2_32" "mbedtls" "mbedcrypto" "mbedx509" "bcrypt")
