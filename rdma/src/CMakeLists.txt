
add_executable(printInfo printInfo.cc)
add_executable(echo echo.cc)


target_link_libraries(printInfo ibverbs)
target_link_libraries(echo ibverbs)