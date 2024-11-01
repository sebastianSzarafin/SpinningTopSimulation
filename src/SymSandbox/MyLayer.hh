#ifndef SYM_BASE_MYLAYER_HH
#define SYM_BASE_MYLAYER_HH

#include "SymBase.hh"

#include "SimulationContext.hh"

using namespace sym_base;

namespace sym
{
  class MyLayer : public Layer
  {
   public:
    MyLayer()
    {
      BufferLayout layout = { { SharedDataType::Float3, "a_Position" } };

      Vertex vertices[8] = {
        { { -.5f, -.5f, .5f } },  { { .5f, -.5f, .5f } },  { { .5f, .5f, .5f } },  { { -.5f, .5f, .5f } },
        { { -.5f, -.5f, -.5f } }, { { .5f, -.5f, -.5f } }, { { .5f, .5f, -.5f } }, { { -.5f, .5f, -.5f } },
      };
      auto vertex_buffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));
      vertex_buffer->set_layout(layout);

      uint32_t indices[] = { 0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 3, 7, 2, 6 };
      auto index_buffer  = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));

      m_cube.m_va = std::make_shared<VertexArray>();
      m_cube.m_va->add_vertex_buffer(vertex_buffer);
      m_cube.m_va->set_index_buffer(index_buffer);

      m_cube.m_shader = std::make_shared<Shader>("shaders/cube.glsl");
    }
    ~MyLayer() = default;

    void update(float dt) override
    {
      auto camera = SimulationContext::s_camera;

      m_cube.m_shader->bind();
      m_cube.m_shader->upload_uniform_float3("u_Color", m_cube.m_color);

      m_cube.m_rotation = glm::angleAxis(dt / 2, glm::vec3(0, 1, 0)) * m_cube.m_rotation;
      auto deviation    = glm::mat4_cast(glm::angleAxis(glm::radians(20.f), glm::vec3(0, 0, 1)));
      auto mvp          = camera->get_projection() * camera->get_view() * deviation * m_cube.get_model_mat();

      m_cube.m_shader->upload_uniform_mat4("u_MVP", mvp);
      RenderCommand::set_draw_primitive(DrawPrimitive::LINES);
      RenderCommand::set_line_width(2);
      Renderer::submit(m_cube.m_va);
      m_cube.m_va->unbind();
    }

    virtual void imgui_update(float dt)
    {
      ImGui::Begin("Settings");
      ImGui::ColorEdit3("Cube color", glm::value_ptr(m_cube.m_color));
      ImGui::End();
    }

   private:
    struct Vertex
    {
      glm::vec3 m_position;
    };

    struct
    {
      std::shared_ptr<VertexArray> m_va;
      std::shared_ptr<Shader> m_shader;
      glm::vec3 m_translation = { 0, sqrt(3) / 2, 0 };
      glm::quat m_rotation = glm::angleAxis((float)(M_PI / 2 - atan(1 / sqrt(2))), glm::normalize(glm::vec3(1, 0, 1)));
      glm::vec3 m_color    = { 1, 1, 0 };
      float m_scale        = 1.f;

      glm::mat4 get_model_mat()
      {
        glm::mat4 model = glm::mat4(1);
        model           = glm::translate(model, m_translation);
        model *= glm::mat4_cast(m_rotation);
        model = glm::scale(model, glm::vec3(m_scale));

        return model;
      }
    } m_cube;
  };
} // namespace sym

#endif // SYM_BASE_MYLAYER_HH
