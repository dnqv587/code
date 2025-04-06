add_executable(producer producer.cc)
add_executable(topic topic.cc)
add_executable(consumer consumer.cc)
target_link_libraries(producer PUBLIC rdkafka)
target_link_libraries(topic PUBLIC rdkafka)
target_link_libraries(consumer PUBLIC rdkafka)

set_target_properties(producer PROPERTIES LINK_FLAGS "-Wl,--disable-new-dtags,-rpath,$ENV{HOME}/lib")
set_target_properties(consumer PROPERTIES LINK_FLAGS "-Wl,--disable-new-dtags,-rpath,$ENV{HOME}/lib")