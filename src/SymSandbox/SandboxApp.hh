#ifndef SYM_BASE_SANDBOXAPP_HH
#define SYM_BASE_SANDBOXAPP_HH

#include "SymBase.hh"

#include "GuiLayer.hh"
#include "InputLayer.hh"
#include "SimulationData.hh"
#include "SimulationLayer.hh"

using namespace sym_base;

namespace sym
{
  class SandboxApp : public Application
  {
   public:
    SandboxApp(const ApplicationParams& params) : Application(params)
    {
      // create simulation
      m_cube                    = std::make_shared<Cube>(SimulationData::s_side_len,
                                      SimulationData::s_density,
                                      glm::angleAxis(glm::radians(SimulationData::s_deviation), glm::vec3(0, 0, 1)));
      SimulationContext::s_cube = m_cube.get();
      create_simulation(SimulationData::s_dt, [&]() { m_cube->update(SimulationData::s_dt); }, LoopStatus::running);

      // create application layers
      push_layer(new InputLayer());
      push_layer(new GuiLayer());
      push_layer(new SimulationLayer());
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

   private:
    std::shared_ptr<Cube> m_cube;
  };
} // namespace sym

#endif // SYM_BASE_SANDBOXAPP_HH
