#ifndef SYM_BASE_RENDERINGCONTEXT_HH
#define SYM_BASE_RENDERINGCONTEXT_HH

struct GLFWwindow;

namespace sym_base
{
  class RenderingContext
  {
   public:
    RenderingContext(GLFWwindow* window_handle);

    void init();
    void swap_buffers();

   private:
    GLFWwindow* m_window_handle;
  };
} // namespace sym_base

#endif // SYM_BASE_RENDERINGCONTEXT_HH