#ifndef SYM_BASE_LAYERS_LAYER_HH
#define SYM_BASE_LAYERS_LAYER_HH

#include "Core/Events/Events.hh"

namespace sym_base
{
  /// @brief Abstract interface for engine's layer. It can ancaplulate part of engine's functionality.
  class Layer
  {
   public:
    Layer() {}

    /// @brief Virtual destructor.
    virtual ~Layer()
    {
      for (auto& child : m_children)
      {
        delete child;
      }
    }

    /// @brief Enables layer.
    virtual void attach() {}
    /// @brief Disables layer.
    virtual void detach() {}
    /// @brief Runs layer logic.
    /// @param dt Time that has passed since last update.
    virtual void update(float dt)
    {
      for (auto child : m_children)
      {
        child->update(dt);
      }
    }
    /// @brief Runs layer logic when rendering imgui.
    /// @param dt Time that has passed since last update.
    virtual void imgui_update(float dt)
    {
      for (auto child : m_children)
      {
        child->imgui_update(dt);
      }
    }
    /// @brief Handle event by this layer.
    /// @param event Event to be handled.
    /// @param dt Time that has passed since last update.
    virtual void handle_event(Event& event, float dt) {}

   protected:
    virtual void push_child_layer(Layer* layer) { m_children.emplace_back(layer); }

   protected:
    std::vector<Layer*> m_children;
  };
} // namespace sym_base

#endif // SYM_BASE_LAYERS_LAYER_HH
