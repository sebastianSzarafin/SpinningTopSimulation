#ifndef SYM_BASE_MYLAYER_HH
#define SYM_BASE_MYLAYER_HH

#include "SymBase.hh"

#include "Cube.hh"
#include "SimulationContext.hh"

using namespace sym_base;

namespace sym
{
  class MyLayer : public Layer
  {
   public:
    MyLayer()
    {
      // cube shader
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

      // line shader
      {
        BufferLayout layout = { { SharedDataType::Float3, "a_Position" } };

        Vertex vertices[2] = { { { 0, 0, 0 } }, { { 0, 0, 0 } } };
        auto vertex_buffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices), BufferType::DYNAMIC);
        vertex_buffer->set_layout(layout);

        uint32_t indices[] = { 0, 1 };
        auto index_buffer  = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));

        m_diag_line.m_va = std::make_shared<VertexArray>();
        m_diag_line.m_va->add_vertex_buffer(vertex_buffer);
        m_diag_line.m_va->set_index_buffer(index_buffer);

        m_diag_line.m_shader = std::make_shared<Shader>("shaders/line.glsl");
      }
    }
    ~MyLayer() = default;

    void update(float dt) override
    {
      auto camera = SimulationContext::s_camera;
      auto cube   = SimulationContext::s_cube;
      auto vp     = camera->get_projection() * camera->get_view();

      m_cube.m_shader->bind();
      m_cube.m_shader->upload_uniform_float3("u_Color", m_cube.m_color);
      m_cube.m_shader->upload_uniform_mat4("u_MVP", vp * cube->get_model_mat());
      RenderCommand::set_draw_primitive(DrawPrimitive::LINES);
      RenderCommand::set_line_width(2);
      Renderer::submit(m_cube.m_va);
      m_cube.m_va->unbind();
      m_cube.m_shader->unbind();

      glm::vec3 mass_centre = 2.f * cube->get_mass_centre();

      m_diag_line.m_shader->bind();
      m_diag_line.m_shader->upload_uniform_float3("u_Color", m_diag_line.m_color);
      m_diag_line.m_shader->upload_uniform_mat4("u_MVP", vp);
      m_diag_line.m_va->get_vertex_buffer(0)->send_data(sizeof(glm::vec3), sizeof(glm::vec3), &mass_centre);
      RenderCommand::set_draw_primitive(DrawPrimitive::LINES);
      RenderCommand::set_line_width(2);
      Renderer::submit(m_diag_line.m_va);
      m_diag_line.m_va->unbind();
      m_diag_line.m_shader->unbind();
    }

    virtual void imgui_update(float dt) {}

   private:
    struct Vertex
    {
      glm::vec3 m_position;
    };

    struct
    {
      std::shared_ptr<VertexArray> m_va;
      std::shared_ptr<Shader> m_shader;
      glm::vec3 m_color = { 1, 1, 0 };
    } m_cube;

    struct
    {
      std::shared_ptr<VertexArray> m_va;
      std::shared_ptr<Shader> m_shader;
      glm::vec3 m_color = { 1, 0, 1 };
    } m_diag_line;
  };
} // namespace sym

#endif // SYM_BASE_MYLAYER_HH
