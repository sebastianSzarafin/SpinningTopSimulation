#ifndef SIMULATIONAPP_CUBE_HH
#define SIMULATIONAPP_CUBE_HH

#include "SymBase.hh"

#include "pch.hh"

using namespace sym_base;

namespace sym
{
  class Cube
  {
   public:
    Cube(float side_length, float density, glm::quat deviation);

    void update(float dt);
    glm::mat4 get_model_mat();
    inline glm::vec3 get_mass_centre() const { return m_Q * m_init_cm * glm::conjugate(m_Q); }

   private:
    const float m_side_length;
    const float m_density;
    const float m_mass;

    glm::vec3 m_init_translation; //
    glm::quat m_init_rotation;    // model
    glm::vec3 m_scale;            //

    glm::quat m_Q;
    glm::vec3 m_W;
    glm::vec3 m_init_cm; // initial mass centre

    glm::mat3 m_I;
    glm::mat3 m_inv_I;
  };
} // namespace sym

#endif // SIMULATIONAPP_CUBE_HH
