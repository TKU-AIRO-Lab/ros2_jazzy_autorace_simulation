#include "turtlebot3_gazebo/traffic_bar_plugin.hpp"

#include <gz/msgs/double.pb.h>
#include <gz/plugin/Register.hh>

#include <chrono>
#include <iostream>

namespace turtlebot3_gazebo
{

TrafficBarPlugin::TrafficBarPlugin()
: gzNode(std::make_unique<gz::transport::Node>())
{
}

void TrafficBarPlugin::Configure(
  const gz::sim::Entity &_entity,
  const std::shared_ptr<const sdf::Element> &_sdf,
  gz::sim::EntityComponentManager &_ecm,
  gz::sim::EventManager &)
{
  this->model = gz::sim::Model(_entity);

  if (!this->model.Valid(_ecm)) {
    std::cerr << "[TrafficBarPlugin] Invalid model entity." << std::endl;
    return;
  }

  this->barJoint = this->model.JointByName(_ecm, "bar_joint");
  if (this->barJoint == gz::sim::kNullEntity) {
    std::cerr << "[TrafficBarPlugin] Failed to find joint: bar_joint" << std::endl;
    return;
  }

  if (_sdf) {
    if (_sdf->HasElement("topic")) {
      this->topic = _sdf->Get<std::string>("topic");
    }
    if (_sdf->HasElement("down_position")) {
      this->downPosition = _sdf->Get<double>("down_position");
    }
    if (_sdf->HasElement("up_position")) {
      this->upPosition = _sdf->Get<double>("up_position");
    }
    if (_sdf->HasElement("down_duration")) {
      this->downDuration = _sdf->Get<double>("down_duration");
    }
    if (_sdf->HasElement("up_duration")) {
      this->upDuration = _sdf->Get<double>("up_duration");
    }
    if (_sdf->HasElement("start_open")) {
      this->isOpen = _sdf->Get<bool>("start_open");
    }
  }

  this->publisher = this->gzNode->Advertise<gz::msgs::Double>(this->topic);

  std::cout << "[TrafficBarPlugin] Loaded successfully for model: "
            << this->model.Name(_ecm) << std::endl;
  std::cout << "[TrafficBarPlugin] topic=" << this->topic
            << ", down=" << this->downPosition
            << ", up=" << this->upPosition
            << ", down_duration=" << this->downDuration
            << ", up_duration=" << this->upDuration
            << ", start_open=" << (this->isOpen ? "true" : "false")
            << std::endl;

  this->lastSimTime = std::chrono::steady_clock::duration::zero();

  if (this->isOpen) {
    this->PublishTarget(this->upPosition);
    std::cout << "[TrafficBarPlugin] initial state => UP" << std::endl;
  } else {
    this->PublishTarget(this->downPosition);
    std::cout << "[TrafficBarPlugin] initial state => DOWN" << std::endl;
  }
}

double TrafficBarPlugin::CurrentStateDuration() const
{
  return this->isOpen ? this->upDuration : this->downDuration;
}

void TrafficBarPlugin::PublishTarget(double position)
{
  if (!this->publisher) {
    std::cerr << "[TrafficBarPlugin] Publisher is not valid." << std::endl;
    return;
  }

  gz::msgs::Double msg;
  msg.set_data(position);

  this->publisher.Publish(msg);

  std::cout << "[TrafficBarPlugin] publish target=" << position
            << " to " << this->topic << std::endl;
}

void TrafficBarPlugin::PreUpdate(
  const gz::sim::UpdateInfo &_info,
  gz::sim::EntityComponentManager &)
{
  if (_info.paused) {
    return;
  }

  const auto currentTime = _info.simTime;
  const auto cycle =
    std::chrono::duration_cast<std::chrono::steady_clock::duration>(
      std::chrono::duration<double>(this->CurrentStateDuration()));

  if ((currentTime - this->lastSimTime) < cycle) {
    return;
  }

  this->isOpen = !this->isOpen;

  if (this->isOpen) {
    this->PublishTarget(this->upPosition);
    std::cout << "[TrafficBarPlugin] state => UP" << std::endl;
  } else {
    this->PublishTarget(this->downPosition);
    std::cout << "[TrafficBarPlugin] state => DOWN" << std::endl;
  }

  this->lastSimTime = currentTime;
}

}  // namespace turtlebot3_gazebo

GZ_ADD_PLUGIN(
  turtlebot3_gazebo::TrafficBarPlugin,
  gz::sim::System,
  gz::sim::ISystemConfigure,
  gz::sim::ISystemPreUpdate
)