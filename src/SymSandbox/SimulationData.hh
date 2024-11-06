#ifndef SIMULATIONAPP_SIMULATIONDATA_HH
#define SIMULATIONAPP_SIMULATIONDATA_HH

#include "SymBase.hh"

namespace sym
{
  static constexpr float G = 9.81f;

  class SimulationData
  {
   public:
    static const glm::vec3 s_gravity;
    static const glm::vec3 s_angular_velocity;
  };

  inline const glm::vec3 SimulationData::s_gravity          = { 0, -G, 0 };
  inline const glm::vec3 SimulationData::s_angular_velocity = { 0, 15, 0 };
} // namespace sym

#endif // SIMULATIONAPP_SIMULATIONDATA_HH
