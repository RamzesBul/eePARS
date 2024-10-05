include(CTest)


# WebAPI test
add_executable(${WEBAPI_TEST} "${CMAKE_SOURCE_DIR}/tests/webapi/api.c")

target_include_directories(${WEBAPI_TEST} PUBLIC ${HEADER_PATH})
target_link_directories(${WEBAPI_TEST} PUBLIC ${LIBRARY_PATH})
target_link_libraries(${WEBAPI_TEST} ${PROJECT_LIB})


# Tests
add_test(NAME ${WEBAPI_TEST} COMMAND $<TARGET_FILE:${WEBAPI_TEST}>)