#ifndef SYM_BASE_SANDBOXAPP_HH
#define SYM_BASE_SANDBOXAPP_HH

#include "SymBase.hh"

#include "GridLayer.hh"
#include "GuiLayer.hh"
#include "InputLayer.hh"
#include "MyLayer.hh"
#include "SimulationContext.hh"

using namespace sym_base;

namespace sym
{
  class SandboxApp : public Application
  {
   public:
    SandboxApp(const ApplicationParams& params) : Application(params)
    {
      push_layer(new InputLayer());
      push_layer(new GuiLayer());
      push_layer(new MyLayer());

      m_camera = std::make_shared<OrbitCamera>();
      m_camera->set_position({ 0, 0, 5 });
      SimulationContext::s_camera = m_camera.get();
    }

    ~SandboxApp() override {}

    virtual void update(float dt) override
    {
      auto& window = Application::get().get_window();
      m_camera->set_perspective(M_PI / 4, window.get_width() / (float)window.get_height(), 1.f, 100.f);

      if (Input::is_key_pressed(GLFW_KEY_ESCAPE)) { m_running = false; }
      Application::update(dt);
    }

   private:
    std::shared_ptr<OrbitCamera> m_camera;
  };
} // namespace sym

#endif // SYM_BASE_SANDBOXAPP_HH
