/*
 * Copyright (C) 2014-2016 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
#include <string.h>
#include "gazebo/test/ServerFixture.hh"

using namespace gazebo;
class SensorTest : public ServerFixture
{
};

/////////////////////////////////////////////////
// This tests getting links from a model.
TEST_F(SensorTest, GetScopedName)
{
  Load("worlds/camera_pose_test.world");

  sensors::SensorPtr sensor = sensors::get_sensor("cam1");
  ASSERT_TRUE(sensor != NULL);

  std::string sensorName = sensor->ScopedName();
  EXPECT_EQ(sensorName, std::string("default::rotated_box::link::cam1"));
}

/////////////////////////////////////////////////
// Make sure sensors can run without asserting in a world with a large
// step size.
TEST_F(SensorTest, FastSensor)
{
  Load("worlds/fast_sensor_test.world");
  physics::WorldPtr world = physics::get_world("default");
  ASSERT_TRUE(world != NULL);

  // This test will cause an assertion if maxSensorUpdate in
  // SensorManager::SensorContainer::RunLoop() is set improperly

  physics::PhysicsEnginePtr physics = world->GetPhysicsEngine();
  ASSERT_TRUE(physics != NULL);
  double dt = physics->GetMaxStepSize();
  EXPECT_DOUBLE_EQ(dt, 0.5);

  // Change time step by several orders of magnitude
  // and verify that there are no assertions
  physics->SetMaxStepSize(1e4);
  common::Time::MSleep(20);
  physics->SetMaxStepSize(1e-3);
  common::Time::MSleep(20);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
