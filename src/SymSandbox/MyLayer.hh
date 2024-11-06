#ifndef SYM_BASE_MYLAYER_HH
#define SYM_BASE_MYLAYER_HH

#include "SymBase.hh"

#include "Cube.hh"
#include "SimulationContext.hh"

using namespace sym_base;

#define SIDE_LEN  1.f
#define DENSITY   1.f
#define DEVIATION 10.f

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

      m_cube.m_cube =
          std::make_shared<Cube>(SIDE_LEN, DENSITY, glm::angleAxis(glm::radians(DEVIATION), glm::vec3(0, 0, 1)));
    }
    ~MyLayer() = default;

    void update(float dt) override
    {
      auto camera = SimulationContext::s_camera;

      m_cube.m_shader->bind();
      m_cube.m_shader->upload_uniform_float3("u_Color", m_cube.m_color);

      m_cube.m_cube->update(dt);
      auto mvp = camera->get_projection() * camera->get_view() * m_cube.m_cube->get_model_mat();

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
      std::shared_ptr<Cube> m_cube;
      glm::vec3 m_color = { 1, 1, 0 };
    } m_cube;
  };
} // namespace sym

#endif // SYM_BASE_MYLAYER_HH
