#ifndef SYM_BASE_GUILAYER_HH
#define SYM_BASE_GUILAYER_HH

#include <cmath>

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

    void update(float dt) override
    {
      m_fps_manager.m_frames_count++;
      m_fps_manager.m_current_time += dt;
      if (m_fps_manager.m_current_time >= m_fps_manager.m_display_time)
      {
        m_fps_manager.m_current_fps  = 1.f / dt;
        m_fps_manager.m_current_time = 0;
      }
    }

    void imgui_update(float dt) override
    {
      ImGui::Begin("Simulation info");
      {
        bool cube_reset = false, simulation_reset = false;

        ImGui::Text("FPS: %.1f", m_fps_manager.m_current_fps);
        ImGui::Text("Simulation time: %.1f", Clock::now());
        ImGui::Spacing();
        /* -------------------- Time step -------------------- */
        {
          static float s_dt = SimulationData::s_dt;
          ImGui::SliderFloat("Time step (dt)", &s_dt, .001f, .2f);
          if (std::fabs(s_dt - SimulationData::s_dt) > FLT_EPSILON && ImGui::IsItemDeactivatedAfterEdit())
          {
            simulation_reset     = true;
            SimulationData::s_dt = s_dt;
          }
          ImGui::Spacing();
        }
        /* -------------------- Angular Velocity -------------------- */
        {
          static float s_av = SimulationData::s_angular_velocity.y;
          ImGui::SliderFloat("Angular velocity (W)", &s_av, 0, 100);
          if (std::fabs(s_av - SimulationData::s_angular_velocity.y) > FLT_EPSILON &&
              ImGui::IsItemDeactivatedAfterEdit())
          {
            cube_reset                         = true;
            SimulationData::s_angular_velocity = { 0, s_av, 0 };
          }
          ImGui::Spacing();
        }
        /* -------------------- Side Length -------------------- */
        {
          static float s_a = SimulationData::s_side_len;
          ImGui::SliderFloat("Side Length (a)", &s_a, .001f, 5);
          if (std::fabs(s_a - SimulationData::s_side_len) > FLT_EPSILON)
          {
            cube_reset                 = true;
            SimulationData::s_side_len = s_a;
          }
          ImGui::Spacing();
        }
        /* -------------------- Density -------------------- */
        {
          static float s_d = SimulationData::s_density;
          ImGui::SliderFloat("Density (Ro)", &s_d, .001f, 100);
          if (std::fabs(s_d - SimulationData::s_density) > FLT_EPSILON && ImGui::IsItemDeactivatedAfterEdit())
          {
            cube_reset                = true;
            SimulationData::s_density = s_d;
          }
          ImGui::Spacing();
        }
        /* -------------------- Deviation -------------------- */
        {
          static float s_d = SimulationData::s_deviation;
          ImGui::SliderFloat("Deviation (alpha)", &s_d, 0, 180);
          if (std::fabs(s_d - SimulationData::s_deviation) > FLT_EPSILON)
          {
            cube_reset                  = true;
            SimulationData::s_deviation = s_d;
          }
          ImGui::Spacing();
        }
        /* -------------------- Trajectory points -------------------- */
        {
          ImGui::InputInt("Trajectory points (n)", &SimulationData::s_trajectory_points);
          SimulationData::s_trajectory_points =
              std::clamp(SimulationData::s_trajectory_points, 0, SimulationData::s_max_trajectory_points);
        }
        /* -------------------- Gravity -------------------- */
        {
          static int gravity    = 1;
          bool gravity_modified = gravity;
          ImGui::Text("Gravity:");
          ImGui::RadioButton("On", &gravity, 1);
          ImGui::SameLine();
          ImGui::RadioButton("Off", &gravity, 0);
          if (gravity != gravity_modified) { SimulationData::s_gravity = { 0, gravity ? -G : 0, 0 }; }
          ImGui::Spacing();
        }
        /* -------------------- Display -------------------- */
        {
          ImGui::Text("Display:");
          ImGui::Checkbox("Cube", &SimulationData::s_display_cube);
          ImGui::Checkbox("Diagonal", &SimulationData::s_display_diag);
          ImGui::Checkbox("Trajectory", &SimulationData::s_display_trajectory);
          ImGui::Spacing();
        }

        if (cube_reset) { SimulationContext::reset_cube(); }

        static bool simulation_paused = false;
        if ((SimulationData::s_reset_button_pressed = ImGui::Button("Reset")) || simulation_reset)
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
        else if ((SimulationData::s_pause_button_pressed = ImGui::Button("Pause")))
        {
          Application::get().pause_simulation();
          simulation_paused = true;
        }
        ImGui::SameLine();
        if ((SimulationData::s_start_button_pressed = ImGui::Button("Start")))
        {
          SimulationContext::reset_cube();
          Application::get().start_simulation();
          simulation_paused = false;
        }
      }
      ImGui::End();

      // ImGui::ShowDemoWindow();
    }

   private:
    struct
    {
      int m_frames_count         = 0;
      float m_current_fps        = 0;
      float m_current_time       = 0;
      const float m_display_time = .25f;
    } m_fps_manager;
  };
} // namespace sym

#endif // SYM_BASE_GUILAYER_HH
