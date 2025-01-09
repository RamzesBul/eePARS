add_executable(${PROJECT_APP} "${CMAKE_SOURCE_DIR}/main.c")

target_include_directories(${PROJECT_APP} PRIVATE ${HEADER_PATH})
target_link_libraries(${PROJECT_APP} PRIVATE ${PROJECT_LIB})