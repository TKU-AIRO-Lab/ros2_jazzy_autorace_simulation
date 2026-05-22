#ifndef TURTLEBOT3_GAZEBO__TRAFFIC_BAR_PLUGIN_HPP_
#define TURTLEBOT3_GAZEBO__TRAFFIC_BAR_PLUGIN_HPP_

#include <chrono>
#include <memory>
#include <string>

#include <gz/transport/Node.hh>
#include <gz/sim/System.hh>
#include <gz/sim/Model.hh>
#include <gz/sim/Entity.hh>

namespace turtlebot3_gazebo
{

class TrafficBarPlugin
  : public gz::sim::System,
    public gz::sim::ISystemConfigure,
    public gz::sim::ISystemPreUpdate
{
public:
  TrafficBarPlugin();
  ~TrafficBarPlugin() override = default;

  void Configure(
    const gz::sim::Entity &_entity,
    const std::shared_ptr<const sdf::Element> &_sdf,
    gz::sim::EntityComponentManager &_ecm,
    gz::sim::EventManager &_eventMgr) override;

  void PreUpdate(
    const gz::sim::UpdateInfo &_info,
    gz::sim::EntityComponentManager &_ecm) override;

private:
  double CurrentStateDuration() const;
  void PublishTarget(double position);

private:
  gz::sim::Model model{gz::sim::kNullEntity};
  gz::sim::Entity barJoint{gz::sim::kNullEntity};

  std::string topic{"/model/traffic_bar/joint/bar_joint/0/cmd_pos"};

  bool isOpen{false};

  double downPosition{0.0};
  double upPosition{-1.5708};

  double downDuration{5.0};
  double upDuration{10.0};

  std::chrono::steady_clock::duration lastSimTime{
    std::chrono::steady_clock::duration::zero()};

  std::unique_ptr<gz::transport::Node> gzNode;
  gz::transport::Node::Publisher publisher;
};

}  // namespace turtlebot3_gazebo

#endif  // TURTLEBOT3_GAZEBO__TRAFFIC_BAR_PLUGIN_HPP_