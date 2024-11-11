#ifndef SIMULATIONAPP_SIMULATIONLAYER_HH
#define SIMULATIONAPP_SIMULATIONLAYER_HH

#include "SymBase.hh"

#include "GridLayer.hh"
#include "MyLayer.hh"
#include "SimulationContext.hh"

using namespace sym_base;

namespace sym
{
  class SimulationLayer : public Layer
  {
   public:
    SimulationLayer()
    {
      // framebuffer
      {
        float vertices[] = {
          // Coords   // TexCoords
          -1.f, -1.f, 0.f, 0.f, //
          1.f,  -1.f, 1.f, 0.f, //
          1.f,  1.f,  1.f, 1.f, //
          -1.f, 1.f,  0.f, 1.f  //
        };

        auto vertex_buffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices), sizeof(float));

        BufferLayout layout = { { SharedDataType::Float2, "a_Position" }, { SharedDataType::Float2, "a_TexCoord" } };
        vertex_buffer->set_layout(layout);

        uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
        auto index_buffer   = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));

        m_framebuffer.m_va = std::make_shared<VertexArray>();
        m_framebuffer.m_va->add_vertex_buffer(vertex_buffer);
        m_framebuffer.m_va->set_index_buffer(index_buffer);

        m_framebuffer.m_shader = std::make_shared<Shader>("shaders/framebuffer.glsl");

        m_framebuffer.m_texture = std::make_shared<Texture2D>(m_framebuffer.m_width, m_framebuffer.m_height);

        m_framebuffer.m_buffer = std::make_shared<Framebuffer>();
        m_framebuffer.m_buffer->set_color_buffer(m_framebuffer.m_texture);
        m_framebuffer.m_buffer->create_render_buffer();
      }

      push_child_layer(new GridLayer());
      push_child_layer(new MyLayer());
    }
    ~SimulationLayer() = default;

    void update(float dt) override
    {
      auto& window           = Application::get().get_window();
      auto rendering_context = window.get_rendering_context();

      SimulationContext::s_camera->set_perspective(M_PI / 4,
                                                   window.get_width() / (float)window.get_height(),
                                                   1.f,
                                                   100.f);

      Renderer::begin_scene();
      {
        rendering_context->set_viewport(0, 0, m_framebuffer.m_width, m_framebuffer.m_height);

        m_framebuffer.m_buffer->bind();
        {
          // clear buffer
          RenderCommand::set_clear_color({ .1f, .1f, .1f, 1.f });
          RenderCommand::clear();
          // enable rendering features
          RenderCommand::depth_buffering(true);
          RenderCommand::anti_aliasing(true);
          RenderCommand::face_culling(true);
          RenderCommand::alpha_blending(true);
          // update children layers
          Layer::update(dt);
        }
        m_framebuffer.m_buffer->unbind();
      }
      Renderer::end_scene();
    }

    void imgui_update(float dt)
    {
      Layer::imgui_update(dt);

      auto window_size = ImVec2(m_framebuffer.m_width, m_framebuffer.m_height);
      ImGui::SetNextWindowSize(window_size);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
      ImGui::Begin("Simulation window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
      ImGui::Image((ImTextureID)(intptr_t)m_framebuffer.m_texture->get_id(), window_size, ImVec2(0, 1), ImVec2(1, 0));
      ImGui::End();
      ImGui::PopStyleVar();
    }

    struct
    {
      std::shared_ptr<VertexArray> m_va;
      std::shared_ptr<Shader> m_shader;
      std::shared_ptr<Texture2D> m_texture;
      std::shared_ptr<Framebuffer> m_buffer;
      const uint32_t m_width  = 800;
      const uint32_t m_height = 600;
    } m_framebuffer;
  };
} // namespace sym

#endif // SIMULATIONAPP_SIMULATIONLAYER_HH
