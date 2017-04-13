#pragma once
#include <controller_manager/controller_manager.h>
#include <hardware_interface/force_torque_sensor_interface.h>
#include <hardware_interface/joint_command_interface.h>
#include <hardware_interface/joint_state_interface.h>
#include <hardware_interface/robot_hw.h>
#include "ur_modern_driver/ros/hardware_interface.h"

using hardware_interface::RobotHW;
using hardware_interface::ControllerInfo;

class RobotHardware : public RobotHW
{
private:
  JointInterface joint_interface_;
  WrenchInterface wrench_interface_;
  PositionInterface position_interface_;
  VelocityInterface velocity_interface_;
  hardware_interface::ForceTorqueSensorInterface force_torque_interface_;
  HardwareInterface* active_interface_;
  std::vector<void*> available_interfaces_;

public:
  RobotHardware(URCommander& commander, std::vector<std::string>& joint_names)
    : joint_interface_(joint_names)
    , wrench_interface_()
    , position_interface_(commander, joint_interface_, joint_names)
    , velocity_interface_(commander, joint_interface_, joint_names)
    , available_interfaces_{&position_interface_, &velocity_interface_}
  {
    registerInterface<hardware_interface::JointStateInterface>(&joint_interface_);
    registerInterface<hardware_interface::ForceTorqueSensorInterface>(&wrench_interface_);
    registerInterface<hardware_interface::PositionJointInterface>(&position_interface_);
    registerInterface<hardware_interface::VelocityJointInterface>(&velocity_interface_);
  }

  //bool canSwitch(const std::list<ControllerInfo>& start_list, const std::list<ControllerInfo>& stop_list) const;
  void doSwitch(const std::list<ControllerInfo>& start_list, const std::list<ControllerInfo>& stop_list);


  /// \brief Read the state from the robot hardware.
  virtual void read(RTShared& packet);

  /// \brief write the command to the robot hardware.
  virtual void write();
};