#ifndef SYM_BASE_SANDBOXAPP_HH
#define SYM_BASE_SANDBOXAPP_HH

#include "SymBase.hh"

#include "FramebufferLayer.hh"
#include "GuiLayer.hh"
#include "InputLayer.hh"
#include "SimulationData.hh"

using namespace sym_base;

namespace sym
{
  class SandboxApp : public Application
  {
   public:
    SandboxApp(const ApplicationParams& params) : Application(params)
    {
      // create simulation
      SimulationContext::s_camera = std::make_shared<OrbitCamera>();
      SimulationContext::s_camera->set_position({ 0, 5, 0 });
      SimulationContext::reset_cube();
      create_simulation(
          SimulationData::s_dt,
          [&]() { SimulationContext::s_cube->update(SimulationData::s_dt); },
          LoopStatus::running);

      // create application layers
      push_layer(new InputLayer());
      push_layer(new GuiLayer());
      push_layer(new FramebufferLayer());
    }

    ~SandboxApp() override {}

    virtual void update(float dt) override
    {
      if (Input::is_key_pressed(GLFW_KEY_ESCAPE))
      {
        m_running = false;
        m_simulation_loop->set_status(LoopStatus::exiting);
      }
      Application::update(dt);
    }
  };
} // namespace sym

#endif // SYM_BASE_SANDBOXAPP_HH
