add_executable(${PROJECT_APP} "${CMAKE_SOURCE_DIR}/main.c")

target_include_directories(${PROJECT_APP} PUBLIC ${HEADER_PATH})
target_link_directories(${PROJECT_APP} PUBLIC ${LIBRARY_PATH})
target_link_libraries(${PROJECT_APP} ${PROJECT_LIB})

target_compile_definitions(${PROJECT_APP} PUBLIC -DMG_ENABLE_MBEDTLS=1)
target_compile_options(${PROJECT_APP} PUBLIC -Wdiscarded-qualifiers)