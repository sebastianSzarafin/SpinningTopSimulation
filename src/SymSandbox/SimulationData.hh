#ifndef SIMULATIONAPP_SIMULATIONDATA_HH
#define SIMULATIONAPP_SIMULATIONDATA_HH

#include "SymBase.hh"

#define G 9.81f

namespace sym
{
  struct SimulationData
  {
    static float s_dt;
    static glm::vec3 s_gravity;
    static glm::vec3 s_angular_velocity;
    static float s_side_len;
    static float s_density;
    static float s_deviation;

    static int s_trajectory_points;
  };

  inline float SimulationData::s_dt                   = .005f;
  inline glm::vec3 SimulationData::s_gravity          = { 0, -G, 0 };
  inline glm::vec3 SimulationData::s_angular_velocity = { 0, 50, 0 };
  inline float SimulationData::s_side_len             = 1.f;
  inline float SimulationData::s_density              = 1.f;
  inline float SimulationData::s_deviation            = 10.f;
  inline int SimulationData::s_trajectory_points      = 100;
} // namespace sym

#endif // SIMULATIONAPP_SIMULATIONDATA_HH
