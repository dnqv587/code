add_executable(zookeeperTest zookeeperTest.cc)

add_library(zookeeperHelper SHARED zookeeperHelper.cc)

target_link_libraries(zookeeperHelper PRIVATE zookeeper hashtable ssl crypto sasl2)

target_link_libraries(zookeeperTest PRIVATE  zookeeperHelper ssl crypto sasl2)