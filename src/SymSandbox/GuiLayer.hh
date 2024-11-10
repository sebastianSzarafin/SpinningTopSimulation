#ifndef SYM_BASE_GUILAYER_HH
#define SYM_BASE_GUILAYER_HH

#include "SymBase.hh"

#include "SimulationContext.hh"

using namespace sym_base;
namespace sym
{
  class GuiLayer : public Layer
  {
   public:
    GuiLayer()  = default;
    ~GuiLayer() = default;

    void imgui_update(float dt) override
    {
      ImGui::Begin("Settings");
      static bool simulation_paused = false;
      if (ImGui::Button("Reset"))
      {
        SimulationContext::reset_cube();
        Application::get().reset_simulation();
        simulation_paused = false;
      }
      ImGui::SameLine();
      if (simulation_paused)
      {
        if (ImGui::Button("Resume"))
        {
          Application::get().resume_simulation();
          simulation_paused = false;
        }
      }
      else if (ImGui::Button("Pause"))
      {
        Application::get().pause_simulation();
        simulation_paused = true;
      }
      ImGui::SameLine();
      if (ImGui::Button("Start"))
      {
        SimulationContext::reset_cube();
        Application::get().start_simulation();
        simulation_paused = false;
      }
      ImGui::End();
    }
  };
} // namespace sym

#endif // SYM_BASE_GUILAYER_HH
