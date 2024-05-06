#include "patrol_behavior.h"
#include "yaml-cpp/yaml.h"
#include <string>
#include <future>

Patrol::Patrol(const std::string &name,
                   const BT::NodeConfiguration &config,
                   rclcpp::Node::SharedPtr node_ptr)
    : BT::StatefulActionNode(name, config), node_ptr_(node_ptr)
{
  action_client_ptr_ = rclcpp_action::create_client<NavigateToPose>(node_ptr_, "/navigate_to_pose");
  done_flag_ = false;
  tf_buffer_ = std::make_unique<tf2_ros::Buffer>(node_ptr_->get_clock());
  tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
}

BT::PortsList Patrol::providedPorts()
{
  BT::PortsList list;
  list.insert(BT::InputPort<int>("loc_in"));
  list.insert(BT::OutputPort<int>("loc_out"));
  list.insert(BT::OutputPort<geometry_msgs::msg::TransformStamped>("cart_loc_out"));
  return list;
}

BT::NodeStatus Patrol::onStart()
{
  // Get location key from port and read YAML file
  auto loc = getInput<int>("loc_in");
  std::string index = location + std::to_string(loc.value());
  count = loc.value();
  if(count<20)
  {
    count++;
  } else{
    count = 1;
  }
  setOutput("loc_out", count);

  const std::string location_file = node_ptr_->get_parameter("location_file").as_string();
  YAML::Node locations = YAML::LoadFile(location_file);
  std::vector<float> pose = locations[index].as<std::vector<float>>();

  // setup action client
  auto send_goal_options = rclcpp_action::Client<NavigateToPose>::SendGoalOptions();
  send_goal_options.result_callback = std::bind(&Patrol::nav_to_pose_callback, this, std::placeholders::_1);

  // make pose
  auto goal_msg = NavigateToPose::Goal();
  goal_msg.pose.header.frame_id = "map";
  goal_msg.pose.pose.position.x = pose[0];
  goal_msg.pose.pose.position.y = pose[1];

  tf2::Quaternion q;
  q.setRPY(0, 0, pose[2]);
  q.normalize(); // todo: why?
  goal_msg.pose.pose.orientation = tf2::toMsg(q);

  // send pose
  done_flag_ = false;
  action_client_ptr_->async_send_goal(goal_msg, send_goal_options);
  RCLCPP_INFO(node_ptr_->get_logger(), "Sent Goal to Nav2\n");
  return BT::NodeStatus::RUNNING;
}

BT::NodeStatus Patrol::onRunning()
{
  geometry_msgs::msg::TransformStamped t;

  try {
        t = tf_buffer_->lookupTransform(
          "marker_0", "camera_color_optical_frame",
          tf2::TimePointZero);
      } catch (const tf2::TransformException & ex) {
        RCLCPP_INFO(
                   node_ptr_->get_logger(), "Could not transform %s to %s: %s",
            "marker_0", "camera_color_optical_frame", ex.what());
      }

  if(t.header.stamp.sec != 0)
  {
    // RCLCPP_INFO(node_ptr_->get_logger(), "Got TF!!!!!!!\n");
    // std::shared_future<GoalHandleNav::SharedPtr> future;
    // while(true)
    // {
    //   if(future.valid())
    //   {
    //     auto goal_handle = future.get();
    //     action_client_ptr_->async_cancel_goal(goal_handle);
    //     break;
    //   }
    // }
    //return BT::NodeStatus::SUCCESS;
    setOutput("cart_loc_out", t);
    return BT::NodeStatus::SUCCESS;
  }

  if (done_flag_)
  {
    RCLCPP_INFO(node_ptr_->get_logger(), "[%s] Goal reached\n", this->name());
    return BT::NodeStatus::FAILURE;
    //return BT::NodeStatus::FAILURE;
  }
  else
  {
    return BT::NodeStatus::RUNNING;
  }
}

void Patrol::nav_to_pose_callback(const GoalHandleNav::WrappedResult &result)
{
  // If there is a result, we consider navigation completed.
  // bt_navigator only sends an empty message without status. Idk why though.

  if (result.result)
  {
    done_flag_ = true;
  }
}

