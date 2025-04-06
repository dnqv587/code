#include <kafka/KafkaConsumer.h>

int main() {
  using namespace kafka;
  using namespace kafka::clients::consumer;
  const std::string brokers = "localhost:9092"; // NOLINT
  const Topic topic = "testTopic";            // NOLINT

  Properties props({{"bootstrap.servers", brokers}});
  props.put(ConsumerConfig::BOOTSTRAP_SERVERS,brokers);
  props.put(ConsumerConfig::GROUP_ID,"dai");
  props.put(ConsumerConfig::ENABLE_AUTO_COMMIT,"false");
  //事务的隔离级别
  //read_committed：确保消费者只能读取已经提交的事务
  //read_uncommitted：允许消费者读取未提交的事务
  props.put(ConsumerConfig::ISOLATION_LEVEL,"read_committed");

  KafkaConsumer consumer(props);

  //订阅
  consumer.subscribe({topic});
  //获取集群信息
  bool flag = true;
  while (flag) {
    consumer.poll(std::chrono::milliseconds(100));
    auto assignment = consumer.assignment();
    if (!assignment.empty()) {
      for (const auto& topicPartition : assignment) {
        if (topicPartition.first == "testTopic") {
          consumer.seek(topicPartition,-1);//重写主题的偏移量
          flag = false;
        }
      }
    }
  }


  //消费消息
  while (true) {
    const auto records = consumer.poll(std::chrono::milliseconds(100));
    for (const auto& record : records) {
      std::cout << static_cast<const char*>(record.value().data()) << std::endl;
    }
    // 手动提交偏移量
    consumer.commitAsync(); //异步提交
    //consumer.commitSync();  //同步提交
  }

  consumer.close();

  return 0;
}