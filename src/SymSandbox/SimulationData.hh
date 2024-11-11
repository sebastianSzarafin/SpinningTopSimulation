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
    static const int s_max_trajectory_points;

    inline static bool reset_button_pressed() { return s_reset_button_pressed; }
    inline static bool pause_button_pressed() { return s_pause_button_pressed; }
    inline static bool start_button_pressed() { return s_start_button_pressed; }

   private:
    static bool s_reset_button_pressed;
    static bool s_pause_button_pressed;
    static bool s_start_button_pressed;

    friend class GuiLayer;
  };

  inline float SimulationData::s_dt                        = .005f;
  inline glm::vec3 SimulationData::s_gravity               = { 0, -G, 0 };
  inline glm::vec3 SimulationData::s_angular_velocity      = { 0, 50, 0 };
  inline float SimulationData::s_side_len                  = 1.f;
  inline float SimulationData::s_density                   = 1.f;
  inline float SimulationData::s_deviation                 = 10.f;
  inline int SimulationData::s_trajectory_points           = 100;
  inline const int SimulationData::s_max_trajectory_points = 5000;
  inline bool SimulationData::s_reset_button_pressed       = false;
  inline bool SimulationData::s_pause_button_pressed       = false;
  inline bool SimulationData::s_start_button_pressed       = false;

} // namespace sym

#endif // SIMULATIONAPP_SIMULATIONDATA_HH
