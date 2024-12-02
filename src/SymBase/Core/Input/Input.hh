#ifndef SYM_BASE_INPUT_HH
#define SYM_BASE_INPUT_HH

namespace sym_base
{
  class Input
  {
   public:
    static bool is_key_pressed(int keycode);
    static bool is_mouse_button_pressed(int button);
  };
} // namespace sym_base

#endif // SYM_BASE_INPUT_HH
