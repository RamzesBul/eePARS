include(CTest)


# WebAPI test
add_executable(${PROJECT_TEST} "${CMAKE_SOURCE_DIR}/tests/webapi/api.c")

target_include_directories(${PROJECT_TEST} PRIVATE ${HEADER_PATH})
target_link_libraries(${PROJECT_TEST} PRIVATE ${PROJECT_LIB})


# Tests
add_test(NAME ${PROJECT_TEST} COMMAND $<TARGET_FILE:${PROJECT_TEST}>)