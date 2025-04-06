#include <kafka/KafkaProducer.h>

#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>


int main()
{
  using namespace kafka;
  using namespace kafka::clients::producer;

  // E.g. KAFKA_BROKER_LIST: "192.168.0.1:9092,192.168.0.2:9092,192.168.0.3:9092"
  const std::string brokers = "localhost:9092"; // NOLINT
  const Topic topic = "testTopic";            // NOLINT

  // Prepare the configuration
  Properties props({{"bootstrap.servers", brokers}});
  props.put(ProducerConfig::ACKS,"-1");
  props.put(ProducerConfig::ENABLE_IDEMPOTENCE,"1");
  props.put(ProducerConfig::BATCH_SIZE,"5");
  props.put(ProducerConfig::REQUEST_TIMEOUT_MS,"3000");
  props.put(ProducerConfig::TRANSACTIONAL_ID,"my-tx-id"); //设置事务ID


  // Create a producer
  KafkaProducer producer(props);

  producer.initTransactions(); //初始化事务
  try {
    producer.beginTransaction(); // 开启事务
    for (int i = 10; i > 0; --i) {
      std::string line = "hello world";
      line += std::to_string(i);

      // Prepare a message
      ProducerRecord record(topic, Key(&i, sizeof(i)),
                                  Value(line.c_str(), line.size()));

      record.setPartition(0);

      // Prepare delivery callback
      // Note: Here we capture the shared pointer of `line`, which holds the
      // content for `record.value()`
      auto deliveryCb = [line](const RecordMetadata& metadata,
                               const Error& error) {
        if (!error) {
          std::cout << "Message delivered: " << metadata.toString() << std::endl;
        } else {
          std::cerr << "Message failed to be delivered: " << error.message()
                    << std::endl;
        }
      };
      // Send the message
      producer.send(record, deliveryCb,KafkaProducer::SendOption::ToCopyRecordValue,KafkaProducer::ActionWhileQueueIsFull::Block);
    }
    producer.commitTransaction(); // 提交事务
  }catch (const std::exception& e) {
    producer.abortTransaction();
    producer.close();
  }

}

