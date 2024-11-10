#ifndef SIMULATIONAPP_SIMULATIONCONTEXT_HH
#define SIMULATIONAPP_SIMULATIONCONTEXT_HH

#include "Cube.hh"
#include "SymBase.hh"

using namespace sym_base;

namespace sym
{
  struct SimulationContext
  {
    static OrbitCamera* s_camera;
    static Cube* s_cube;
  };

  inline OrbitCamera* SimulationContext::s_camera = nullptr;
  inline Cube* SimulationContext::s_cube          = nullptr;
} // namespace sym

#endif // SIMULATIONAPP_SIMULATIONCONTEXT_HH
