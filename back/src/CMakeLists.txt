add_executable(${PROJECT_APP} "${CMAKE_SOURCE_DIR}/main.c")

target_include_directories(${PROJECT_APP} PUBLIC ${HEADER_PATH})
target_link_directories(${PROJECT_APP} PUBLIC ${LIBRARY_PATH})
target_link_libraries(${PROJECT_APP} ${PROJECT_LIB})