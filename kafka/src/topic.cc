#include <rdkafkacpp.h>
#include <iostream>
#include <string>

class ExampleDeliveryReportCb : public RdKafka::DeliveryReportCb {
public:
  void dr_cb(RdKafka::Message &message) {
    /* If message.err() is non-zero the message delivery failed permanently
     * for the message. */
    if (message.err())
      std::cerr << "% Message delivery failed: " << message.errstr()
                << std::endl;
    else
      std::cerr << "% Message delivered to topic " << message.topic_name()
                << " [" << message.partition() << "] at offset "
                << message.offset() << std::endl;
  }
};


int main() {
  RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
  std::string error;
  if (conf->set("bootstrap.servers", "localhost:9092", error) !=
    RdKafka::Conf::CONF_OK) {
    std::cerr << error << std::endl;
    exit(1);
  }
  ExampleDeliveryReportCb ex_dr_cb;

  if (conf->set("dr_cb", &ex_dr_cb, error) != RdKafka::Conf::CONF_OK) {
    std::cerr << error << std::endl;
    exit(1);
  }

  RdKafka::Producer *producer = RdKafka::Producer::create(conf, error);
  if (!producer) {
    std::cerr << "Failed to create producer: " << error << std::endl;
    exit(1);
  }

  auto * topic = RdKafka::Topic::create(producer, "test", conf, error);
  if (!topic) {
    std::cerr << "Failed to create topic: " << error << std::endl;
    exit(1);
  }



  return 0;
}