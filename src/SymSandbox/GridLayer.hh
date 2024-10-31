#ifndef SIMULATIONAPP_GRIDLAYER_HH
#define SIMULATIONAPP_GRIDLAYER_HH

#include "SymBase.hh"

using namespace sym_base;

namespace sym
{
  struct GridVertex
  {
    glm::vec3 m_position;
    glm::vec3 m_color;
  };

  struct GridParams
  {
    int m_threshold;
    float m_scale;
    glm::vec3 m_color;
    float m_y;
  };

  static void generate_grid(std::vector<GridVertex>& vertices, std::vector<uint32_t>& indices, GridParams params);

  class GridLayer : public Layer
  {
   public:
    GridLayer()
    {
      BufferLayout layout = { { SharedDataType::Float3, "a_Position" }, { SharedDataType::Float3, "a_Color" } };

      std::vector<GridVertex> vertices;
      std::vector<uint32_t> indices;

      generate_grid(vertices, indices, { 11, 1, { 1, 1, 1 }, 0 });

      auto vertex_buffer = std::make_shared<VertexBuffer>(vertices.data(), vertices.size() * sizeof(GridVertex));
      vertex_buffer->set_layout(layout);

      auto index_buffer = std::make_shared<IndexBuffer>(indices.data(), indices.size());

      m_grid.m_va = std::make_shared<VertexArray>();
      m_grid.m_va->add_vertex_buffer(vertex_buffer);
      m_grid.m_va->set_index_buffer(index_buffer);

      m_grid.m_shader = std::make_shared<Shader>("shaders/grid.glsl");
    }

    void update(float dt)
    {
      auto camera = SimulationContext::s_camera;

      m_grid.m_shader->bind();
      m_grid.m_shader->upload_uniform_mat4("u_VP", camera->get_projection() * camera->get_view());
      RenderCommand::set_draw_primitive(DrawPrimitive::LINES);
      RenderCommand::set_line_width(1);
      Renderer::submit(m_grid.m_va);
      m_grid.m_shader->unbind();
    }

   private:
    struct
    {
      std::shared_ptr<VertexArray> m_va;
      std::shared_ptr<Shader> m_shader;
    } m_grid;
  };

  static void generate_grid(std::vector<GridVertex>& vertices, std::vector<uint32_t>& indices, GridParams params)
  {
    // OX
    int offset = vertices.size();
    for (int i = 0; i < params.m_threshold; i++)
    {
      GridVertex v{};
      v.m_position = { i - params.m_threshold / 2, 0, 0 };
      v.m_color    = { 1, 0, 0 };

      int idx = i + offset;

      vertices.push_back(v);

      if (i < params.m_threshold - 1)
      {
        indices.push_back(idx);
        indices.push_back(idx + 1);
      }
    }

    // OZ
    offset = vertices.size();
    for (int i = 0; i < params.m_threshold; i++)
    {
      GridVertex v{};
      v.m_position = { 0, 0, i - params.m_threshold / 2 };
      v.m_color    = { 0, 0, 1 };

      int idx = i + offset;

      vertices.push_back(v);

      if (i < params.m_threshold - 1)
      {
        indices.push_back(idx);
        indices.push_back(idx + 1);
      }
    }

    // OY
    offset = vertices.size();
    for (int i = 0; i < params.m_threshold; i++)
    {
      GridVertex v{};
      v.m_position = { 0, i - params.m_threshold / 2, 0 };
      v.m_color    = { 0, 1, 0 };

      int idx = i + offset;

      vertices.push_back(v);

      if (i < params.m_threshold - 1)
      {
        indices.push_back(idx);
        indices.push_back(idx + 1);
      }
    }

    offset = vertices.size();
    for (int i = 0; i < params.m_threshold; i++)
    {
      for (int j = 0; j < params.m_threshold; j++)
      {
        GridVertex v{};
        v.m_position = { i - params.m_threshold / 2, params.m_y, j - params.m_threshold / 2 };
        v.m_color    = params.m_color;

        vertices.push_back(v);
      }
    }

    // columns
    for (int i = 0; i < params.m_threshold; i++)
    {
      for (int j = 0; j < params.m_threshold; j++)
      {
        int idx = i * params.m_threshold + j + offset;

        if (j < params.m_threshold - 1)
        {
          indices.push_back(idx);
          indices.push_back(idx + 1);
        }
      }
    }

    // rows
    for (int i = 0; i < params.m_threshold; i++)
    {
      for (int j = 0; j < params.m_threshold; j++)
      {
        int idx = i * params.m_threshold + j + offset;

        if (i < params.m_threshold - 1)
        {
          indices.push_back(idx);
          indices.push_back(idx + params.m_threshold);
        }
      }
    }

    for (auto& v : vertices)
    {
      v.m_position *= params.m_scale;
    }
  }

} // namespace sym

#endif // SIMULATIONAPP_GRIDLAYER_HH
