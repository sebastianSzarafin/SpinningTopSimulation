#include "Cube.hh"
#include "SimulationData.hh"

static glm::vec3 dWdt(glm::vec3 W, glm::mat3 I, glm::mat3 inv_I, glm::vec3 N);
static glm::quat dQdt(glm::quat Q, glm::vec3 W);
static glm::vec3 calculate_gravity_torque(glm::vec3 radius, float mass);

namespace sym
{
  Cube::Cube(float side_length, float density, glm::quat deviation) :
      m_side_length{ side_length }, m_density{ density }, m_mass{ density * (float)pow(side_length, 3) }
  {
    m_init_translation = { 0, m_side_length * sqrt(3) / 2, 0 };
    m_init_rotation    = glm::angleAxis((float)(M_PI / 2 - atan(1 / sqrt(2))), glm::normalize(glm::vec3(1, 0, 1)));
    m_scale            = { side_length, side_length, side_length };

    m_Q       = { 1, 0, 0, 0 };
    m_W       = SimulationData::s_angular_velocity;
    m_init_cm = deviation * m_init_translation * glm::conjugate(deviation); // TODO:

    // ------------------------ inertia tensor ------------------------
    glm::mat3 I_c(1 / 6.f * m_density * (float)pow(m_side_length, 5) * glm::mat3(1));
    I_c = glm::mat3_cast(m_init_rotation) * I_c * glm::mat3_cast(glm::conjugate(m_init_rotation));

    glm::mat3 I_rc(3 / 4.f * m_density * (float)pow(m_side_length, 5) * glm::mat3(1));
    I_rc[1][1] = 0;

    m_I     = I_rc + I_c;
    m_inv_I = glm::inverse(m_I);
    // ----------------------------------------------------------------
  }

  void Cube::update(float dt)
  {
    glm::vec cm = m_Q * m_init_cm * glm::conjugate(m_Q);
    glm::vec3 N = calculate_gravity_torque(cm, m_mass);

    glm::vec3 k1_W = dWdt(m_W, m_I, m_inv_I, N);
    glm::vec3 k2_W = dWdt(m_W + k1_W * (dt / 2.0f), m_I, m_inv_I, N);
    glm::vec3 k3_W = dWdt(m_W + k2_W * (dt / 2.0f), m_I, m_inv_I, N);
    glm::vec3 k4_W = dWdt(m_W + k3_W * dt, m_I, m_inv_I, N);

    m_W = m_W + (k1_W + 2.0f * k2_W + 2.0f * k3_W + k4_W) * (dt / 6.0f);

    glm::quat k1_Q = dQdt(m_Q, m_W);
    glm::quat k2_Q = dQdt(m_Q + k1_Q * (dt / 2.0f), m_W);
    glm::quat k3_Q = dQdt(m_Q + k2_Q * (dt / 2.0f), m_W);
    glm::quat k4_Q = dQdt(m_Q + k3_Q * dt, m_W);

    m_Q = m_Q + (k1_Q + 2.0f * k2_Q + 2.0f * k3_Q + k4_Q) * (dt / 6.0f);
    m_Q = glm::normalize(m_Q);
  }

  glm::mat4 Cube::get_model_mat()
  {
    glm::mat4 model = glm::mat4(1);
    model           = glm::translate(model, m_init_translation);
    model *= glm::mat4_cast(m_init_rotation);
    model = glm::scale(model, m_scale);
    model = glm::mat4_cast(m_Q) * model;

    return model;
  }
} // namespace sym

static glm::vec3 dWdt(glm::vec3 W, glm::mat3 I, glm::mat3 inv_I, glm::vec3 N)
{
  return inv_I * (N + glm::cross(I * W, W));
}

static glm::quat dQdt(glm::quat Q, glm::vec3 W)
{
  glm::quat W_quat(0.0f, W.x, W.y, W.z);
  return .5f * Q * W_quat;
}

static glm::vec3 calculate_gravity_torque(glm::vec3 radius, float mass)
{
  return glm::cross(radius, mass * sym::SimulationData::s_gravity);
}