#include "turtlebot3_gazebo/traffic_light_plugin.hpp"

#include <gz/plugin/Register.hh>
#include <gz/sim/components/JointPositionReset.hh>

#include <chrono>
#include <iostream>
#include <vector>

namespace turtlebot3_gazebo
{

TrafficLightPlugin::TrafficLightPlugin()
{
}

void TrafficLightPlugin::Configure(
  const gz::sim::Entity &_entity,
  const std::shared_ptr<const sdf::Element> &_sdf,
  gz::sim::EntityComponentManager &_ecm,
  gz::sim::EventManager &)
{
  this->model = gz::sim::Model(_entity);

  if (!this->model.Valid(_ecm)) {
    std::cerr << "[TrafficLightPlugin] Invalid model entity." << std::endl;
    return;
  }

  this->redJoint = this->model.JointByName(_ecm, "red_joint");
  this->yellowJoint = this->model.JointByName(_ecm, "yellow_joint");
  this->greenJoint = this->model.JointByName(_ecm, "green_joint");

  if (this->redJoint == gz::sim::kNullEntity ||
      this->yellowJoint == gz::sim::kNullEntity ||
      this->greenJoint == gz::sim::kNullEntity)
  {
    std::cerr << "[TrafficLightPlugin] Failed to find one or more joints: "
              << "red_joint, yellow_joint, green_joint" << std::endl;
    return;
  }

  if (_sdf) {
    if (_sdf->HasElement("red_duration")) {
      this->redDuration = _sdf->Get<double>("red_duration");
    }
    if (_sdf->HasElement("yellow_duration")) {
      this->yellowDuration = _sdf->Get<double>("yellow_duration");
    }
    if (_sdf->HasElement("green_duration")) {
      this->greenDuration = _sdf->Get<double>("green_duration");
    }
  }

  std::cout << "[TrafficLightPlugin] Loaded successfully for model: "
            << this->model.Name(_ecm) << std::endl;
  std::cout << "[TrafficLightPlugin] durations => red: " << this->redDuration
            << "s, yellow: " << this->yellowDuration
            << "s, green: " << this->greenDuration << "s" << std::endl;

  this->status = 0;
  this->lastSimTime = std::chrono::steady_clock::duration::zero();

  // Initial state: RED
  this->SetState(_ecm, -0.015, 0.0, 0.0);
}

double TrafficLightPlugin::CurrentStateDuration() const
{
  if (this->status == 0) {
    return this->redDuration;
  }
  if (this->status == 1) {
    return this->yellowDuration;
  }
  return this->greenDuration;
}

void TrafficLightPlugin::PreUpdate(
  const gz::sim::UpdateInfo &_info,
  gz::sim::EntityComponentManager &_ecm)
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

  if (this->status == 0) {
    // RED -> YELLOW
    this->status = 1;
    this->SetState(_ecm, 0.0, -0.016, 0.0);
    std::cout << "[TrafficLightPlugin] state => YELLOW" << std::endl;
  } else if (this->status == 1) {
    // YELLOW -> GREEN
    this->status = 2;
    this->SetState(_ecm, 0.0, 0.0, -0.017);
    std::cout << "[TrafficLightPlugin] state => GREEN" << std::endl;
  } else {
    // GREEN -> RED
    this->status = 0;
    this->SetState(_ecm, -0.015, 0.0, 0.0);
    std::cout << "[TrafficLightPlugin] state => RED" << std::endl;
  }

  this->lastSimTime = currentTime;
}

void TrafficLightPlugin::SetState(
  gz::sim::EntityComponentManager &_ecm,
  double redPos,
  double yellowPos,
  double greenPos)
{
  auto setJointPos = [&_ecm](gz::sim::Entity joint, double pos)
  {
    std::vector<double> value{pos};

    auto comp =
      _ecm.Component<gz::sim::components::JointPositionReset>(joint);

    if (comp == nullptr) {
      _ecm.CreateComponent(
        joint,
        gz::sim::components::JointPositionReset(value));
    } else {
      _ecm.RemoveComponent<gz::sim::components::JointPositionReset>(joint);
      _ecm.CreateComponent(
        joint,
        gz::sim::components::JointPositionReset(value));
    }
  };

  if (this->redJoint != gz::sim::kNullEntity) {
    setJointPos(this->redJoint, redPos);
  }
  if (this->yellowJoint != gz::sim::kNullEntity) {
    setJointPos(this->yellowJoint, yellowPos);
  }
  if (this->greenJoint != gz::sim::kNullEntity) {
    setJointPos(this->greenJoint, greenPos);
  }
}

}  // namespace turtlebot3_gazebo

GZ_ADD_PLUGIN(
  turtlebot3_gazebo::TrafficLightPlugin,
  gz::sim::System,
  gz::sim::ISystemConfigure,
  gz::sim::ISystemPreUpdate
)