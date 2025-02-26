# 启用测试
enable_testing()

add_executable(lesson1 lesson1.cc)

#target_link_libraries(lesson1 gmock)
target_link_libraries(lesson1  GTest::gtest_main)

# 包含 GoogleTest 模块
include(GoogleTest)
# 使用 gtest_discover_tests 函数来自动发现并添加测试
gtest_discover_tests(lesson1)


