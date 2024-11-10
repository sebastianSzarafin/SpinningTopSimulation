#ifndef SIMULATIONAPP_SIMULATIONCONTEXT_HH
#define SIMULATIONAPP_SIMULATIONCONTEXT_HH

#include "SymBase.hh"

#include "Cube.hh"
#include "SimulationData.hh"

using namespace sym_base;

namespace sym
{
  struct SimulationContext
  {
    static void reset_cube();

    static std::shared_ptr<OrbitCamera> s_camera;
    static std::shared_ptr<Cube> s_cube;
  };

  inline void SimulationContext::reset_cube()
  {
    s_cube = std::make_shared<Cube>(SimulationData::s_side_len,
                                    SimulationData::s_density,
                                    glm::angleAxis(glm::radians(SimulationData::s_deviation), glm::vec3(0, 0, 1)));
  }

  inline std::shared_ptr<OrbitCamera> SimulationContext::s_camera = nullptr;
  inline std::shared_ptr<Cube> SimulationContext::s_cube          = nullptr;
} // namespace sym

#endif // SIMULATIONAPP_SIMULATIONCONTEXT_HH
