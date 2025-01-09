add_library(${PROJECT_LIB} ${LIB_SOURCES})

target_include_directories(${PROJECT_LIB} PUBLIC ${HEADER_PATH})
target_link_directories(${PROJECT_LIB} PUBLIC ${LIBRARY_PATH})
target_link_libraries(${PROJECT_LIB} PUBLIC ${LIB_BINARIES})