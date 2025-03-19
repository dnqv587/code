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
  delete conf;

  std::string topic = "testTopic";
  std::string value = "hello world!";
  RdKafka::ErrorCode err = producer->produce(
      /* Topic name */
      topic,
      /* Any Partition: the builtin partitioner will be
       * used to assign the message to a topic based
       * on the message key, or random partition if
       * the key is not set. */
      RdKafka::Topic::PARTITION_UA,
      /* Make a copy of the value */
      RdKafka::Producer::RK_MSG_COPY /* Copy payload */,
      /* Value */
      value.data(), value.size(),
      /* Key */
      nullptr, 0,
      /* Timestamp (defaults to current time) */
      0,
      /* Message headers, if any */
      nullptr,
      /* Per-message opaque value passed to
       * delivery report */
      nullptr);

  if (err != RdKafka::ERR_NO_ERROR) {
    std::cerr << "% Failed to produce to topic " << topic << ": "
              << RdKafka::err2str(err) << std::endl;

    if (err == RdKafka::ERR__QUEUE_FULL) {
      /* If the internal queue is full, wait for
       * messages to be delivered and then retry.
       * The internal queue represents both
       * messages to be sent and messages that have
       * been sent or failed, awaiting their
       * delivery report callback to be called.
       *
       * The internal queue is limited by the
       * configuration property
       * queue.buffering.max.messages and queue.buffering.max.kbytes */
      producer->poll(1000 /*block for max 1000ms*/);
    }

  } else {
    std::cerr << "% Enqueued message (" << value.size() << " bytes) "
              << "for topic " << topic << std::endl;
  }
  std::cerr << "% Flushing final messages..." << std::endl;
  producer->flush(10 * 1000 /* wait for max 10 seconds */);

  if (producer->outq_len() > 0)
    std::cerr << "% " << producer->outq_len()
              << " message(s) were not delivered" << std::endl;

  delete producer;
}