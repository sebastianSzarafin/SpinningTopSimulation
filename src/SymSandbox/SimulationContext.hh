#ifndef SIMULATIONAPP_SIMULATIONCONTEXT_HH
#define SIMULATIONAPP_SIMULATIONCONTEXT_HH

#include "SymBase.hh"

using namespace sym_base;

namespace sym
{
  struct SimulationContext
  {
    static OrbitCamera* s_camera;
  };

  inline OrbitCamera* SimulationContext::s_camera = nullptr;
} // namespace sym

#endif // SIMULATIONAPP_SIMULATIONCONTEXT_HH
