# CMake configuration
cmake_minimum_required(VERSION 3.0.0)

project(eePARS VERSION 0.1.0)

# Preprocessor flags
add_definitions(-DMG_ENABLE_MBEDTLS=1)

# Script variables
set(SOURCES "main.c" "src/mongoose.c" "src/client.c" "src/server.c")

# Script properties
add_executable(${PROJECT_NAME} ${SOURCES})

# Headers directory
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/include/mbedtls" "${PROJECT_SOURCE_DIR}/include/mongoose")

# Libraries directory
target_link_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/lib" "${PROJECT_SOURCE_DIR}/lib/mbedtls")

# Link libraries
target_link_libraries(${PROJECT_NAME} ws2_32 mbedtls mbedcrypto mbedx509)
