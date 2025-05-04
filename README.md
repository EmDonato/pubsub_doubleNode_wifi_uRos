# ESP32 Micro-ROS Publisher and Subscriber

This firmware demonstrates a simple publisher and subscriber communication setup using micro-ROS on the ESP32. The ESP32 will publish an integer value incrementing every second and subscribe to a topic to receive integer messages.


The ROS2 program (Server side) is maintained in a separate repository.  [EmDonato/pubsub_doubleNode_template_wifi_ROS2](https://github.com/EmDonato/pubsub_doubleNode_template_wifi_ROS2.git)

## Features
- **Publisher**: Sends an incrementing integer (`Int32`) message every second.
- **Subscriber**: Receives `Int32` messages and prints the value to the serial monitor.
- **Wi-Fi Communication**: The ESP32 is configured to connect to a specified Wi-Fi network for communication with a micro-ROS system.

## Prerequisites

Before you can use this firmware, you need:

- An **ESP32** board.
- **micro-ROS** installed and set up on your machine (refer to [micro-ROS installation](https://micro.ros.org/docs/)).
- A **Wi-Fi network** to connect your ESP32 to.
- **Arduino IDE** or any other compatible environment for ESP32 development.
- **ROS 2** running on your PC or any device that will subscribe/publish to the same topics.

## Setup Instructions

1. **Install Required Libraries**:
   - Install the micro-ROS Arduino library via the Arduino Library Manager or manually.
   
2. **Configure Wi-Fi**:
   - Replace the following Wi-Fi credentials with your own:
     ```cpp
     char wifiNetName[] = "wifi_name";
     char wifiPassword[] = "123123";
     char personalIP[] = "192.168.0.120";
     ```

3. **Micro-ROS Setup**:
   - Make sure that the micro-ROS agent is running and that it can connect to the ESP32 via the same network.
   
4. **Upload the Firmware**:
   - Open the Arduino IDE, select your ESP32 board, and upload the firmware.

## Code Overview

The firmware consists of the following key components:

1. **Publisher**: Sends an `Int32` message incrementing every second:
   ```cpp
   rcl_publisher_t publisher;
   std_msgs__msg__Int32 msg_pub;

2. **Subscriber**: Listens to Int32 messages and prints them to the serial monitor:
   ```cpp
  rcl_subscription_t subscriber;
  std_msgs__msg__Int32 msg_sub;

3. **Timer Callback**: Executes every second to publish an updated Int32 message:
    ```cpp
    void timer_callback(rcl_timer_t * timer, int64_t last_call_time)
    {  
      if (timer != NULL) {
        rcl_publish(&publisher, &msg_pub, NULL);
        msg_pub.data++;
      }
    }

4. **Subscriber Callback**: Executes when a message is received, printing the message data:
    ```cpp
    void subscriber_callback(const void * msgin)
    {
      const std_msgs__msg__Int32 * received_msg = (const std_msgs__msg__Int32 *)msgin;
      Serial.print("Received message: ");
      Serial.println(received_msg->data);
    }

5. **micro-ROS Node Initialization**:
    ```cpp
    Initializes the micro-ROS environment and sets up the node, publisher, subscriber, and timer:

    RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
    RCCHECK(rclc_node_init_default(&node_pub, "micro_ros_esp32_node_pub", "", &support));
    RCCHECK(rclc_publisher_init_default(&publisher, &node_pub, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32), "micro_ros_pub_topic"));

## Running the Firmware

   - **Start the micro-ROS agent**:

        Run the micro-ROS agent on your computer or device that will communicate with the ESP32.

  - **Monitor the Serial Output**:

    Open the Serial Monitor (set to 115200 baud) to observe the published messages and the received messages.

- **Test Communication**:

  The ESP32 will publish messages to the topic micro_ros_pub_topic and subscribe to messages from micro_ros_sub_topic.

## Troubleshooting

 - **Connection Issues**:

      Ensure that the ESP32 is connected to the correct Wi-Fi network.

      Verify that the micro-ROS agent is running and accessible on the network.

    
 - **Topic Mismatch**:

      Make sure that the topic names (micro_ros_pub_topic and micro_ros_sub_topic) are consistent between the ESP32 and the micro-ROS agent.
