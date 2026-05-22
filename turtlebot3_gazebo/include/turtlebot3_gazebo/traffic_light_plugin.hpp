#ifndef TURTLEBOT3_GAZEBO__TRAFFIC_LIGHT_PLUGIN_HPP_
#define TURTLEBOT3_GAZEBO__TRAFFIC_LIGHT_PLUGIN_HPP_

#include <chrono>
#include <memory>

#include <gz/sim/System.hh>
#include <gz/sim/Model.hh>
#include <gz/sim/Entity.hh>

namespace turtlebot3_gazebo
{

class TrafficLightPlugin
  : public gz::sim::System,
    public gz::sim::ISystemConfigure,
    public gz::sim::ISystemPreUpdate
{
public:
  TrafficLightPlugin();
  ~TrafficLightPlugin() override = default;

  void Configure(
    const gz::sim::Entity &_entity,
    const std::shared_ptr<const sdf::Element> &_sdf,
    gz::sim::EntityComponentManager &_ecm,
    gz::sim::EventManager &_eventMgr) override;

  void PreUpdate(
    const gz::sim::UpdateInfo &_info,
    gz::sim::EntityComponentManager &_ecm) override;

private:
  void SetState(
    gz::sim::EntityComponentManager &_ecm,
    double redPos,
    double yellowPos,
    double greenPos);

  double CurrentStateDuration() const;

private:
  gz::sim::Model model{gz::sim::kNullEntity};

  gz::sim::Entity redJoint{gz::sim::kNullEntity};
  gz::sim::Entity yellowJoint{gz::sim::kNullEntity};
  gz::sim::Entity greenJoint{gz::sim::kNullEntity};

  // 0 = RED, 1 = YELLOW, 2 = GREEN
  int status{0};

  double redDuration{1.0};
  double yellowDuration{1.0};
  double greenDuration{1.0};

  std::chrono::steady_clock::duration lastSimTime{
    std::chrono::steady_clock::duration::zero()};
};

}  // namespace turtlebot3_gazebo

#endif  // TURTLEBOT3_GAZEBO__TRAFFIC_LIGHT_PLUGIN_HPP_