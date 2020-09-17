add_executable(test-smoke smoke.c)
target_link_libraries(test-smoke html)

add_test(smoke test-smoke)
