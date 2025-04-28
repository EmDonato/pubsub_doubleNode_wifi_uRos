#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/int32.h>

rcl_publisher_t publisher;
rcl_subscription_t subscriber;

rcl_node_t node_pub, node_sub;
rcl_timer_t timer;

rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;

std_msgs__msg__Int32 msg_pub;
std_msgs__msg__Int32 msg_sub;

//wifi info
char wifiNetName[] = "wifi_name";
char wifiPassword[] = "123123";
char personalIP[] = "192.168.0.120";


#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}



void error_loop(){
  while(1){
    Serial.println("ERROR");
    delay(100);
  }
}

void timer_callback(rcl_timer_t * timer, int64_t last_call_time)
{  
  RCLC_UNUSED(last_call_time);
  if (timer != NULL) {
    RCSOFTCHECK(rcl_publish(&publisher, &msg_pub, NULL));
    msg_pub.data++;
  }
}

void subscriber_callback(const void * msgin)
{
  const std_msgs__msg__Int32 * received_msg = (const std_msgs__msg__Int32 *)msgin;
  Serial.print("Received message: ");
  Serial.println(received_msg->data);
}

void setup() {

  set_microros_wifi_transports(wifiNetName, wifiPassword, personalIP, 8888);// put your wifi
  Serial.begin(115200);  // Aggiungi la comunicazione seriale
  delay(1000);

  allocator = rcl_get_default_allocator();

  // Initialize micro-ROS support
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // Create node
  RCCHECK(rclc_node_init_default(&node_pub, "micro_ros_esp32_node_pub", "", &support));
  RCCHECK(rclc_node_init_default(&node_sub, "micro_ros_esp32_node_sub", "", &support));

  // Create publisher
  RCCHECK(rclc_publisher_init_default(
    &publisher,
    &node_pub,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    "micro_ros_pub_topic"));

  // Create subscriber
  RCCHECK(rclc_subscription_init_default(
    &subscriber,
    &node_sub,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    "micro_ros_sub_topic"));

  // Create timer
  const unsigned int timer_timeout = 1000; //1000 ms --> 1 s
  RCCHECK(rclc_timer_init_default(
    &timer,
    &support,
    RCL_MS_TO_NS(timer_timeout),
    timer_callback));

  // Create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 3, &allocator)); // 2 tasks: timer + subscriber
  RCCHECK(rclc_executor_add_timer(&executor, &timer));
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &msg_sub, &subscriber_callback, ON_NEW_DATA));

  msg_pub.data = 0;
}

void loop() {
  delay(100);
  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
}

