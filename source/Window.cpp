///                                                                           
/// Langulus::Module::GLFW                                                    
/// Copyright(C) 2015 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Window.hpp"
#include "Platform.hpp"
#include "Event.hpp"
#include <GLFW/glfw3native.h>

/// Get native window handle as type-erased void pointer                      
///   @param window - GLFW window interface to extract handle from            
inline void* GetNativeWindowPointer(GLFWwindow* window) {
   #if LANGULUS_OS(WINDOWS)
      return glfwGetWin32Window(window);
   #elif LANGULUS_OS(LINUX)
      return reinterpret_cast<void*>(glfwGetX11Window(window));
   #else
      #error Not implemented for your platform
   #endif
}

///                                                                           
/// Callback predefinitions for various window events                         

void OnClosed(GLFWwindow*);
void OnKeyboardKey(GLFWwindow*, int key, int scancode, int action, int mods);
void OnMove(GLFWwindow*, int x, int y);
void OnResize(GLFWwindow*, int x, int y);
void OnFocus(GLFWwindow*, int focused);
void OnMinimize(GLFWwindow*, int iconified);
void OnResolutionChange(GLFWwindow*, int x, int y);
void OnHover(GLFWwindow*, int entered);
void OnMouseKey(GLFWwindow*, int button, int action, int mods);
void OnMouseScroll(GLFWwindow*, double xoffset, double yoffset);
void OnTextInput(GLFWwindow*, unsigned int codepoint);
void OnFileDrop(GLFWwindow*, int count, const char** paths);


/// Window construction                                                       
///   @param descriptor - window descriptor                                   
Window::Window(const Any& descriptor)
   : Unit {MetaOf<Window>(), descriptor} {
   // Extract properties from descriptor and hierarchy                  
   mOwners.SeekTrait<Traits::Size>(descriptor, mSize);
   mOwners.SeekTrait<Traits::Name>(descriptor, mTitle);

   // Make it visible                                                   
   glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
   glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

   // Create the canvas                                                 
   mGLFWWindow = glfwCreateWindow(
      int(mSize[0]), int(mSize[1]),
      const_cast<char*>(mTitle.Terminate().GetRaw()),
      nullptr, nullptr
   );

   if (!mGLFWWindow)
      LANGULUS_THROW(Construct, "Failed to initialize window");

   // Set the callbacks and user pointers for the canvas pipe           
   glfwSetWindowCloseCallback(mGLFWWindow.Get(), OnClosed);
   glfwSetKeyCallback(mGLFWWindow.Get(), OnKeyboardKey);
   glfwSetWindowPosCallback(mGLFWWindow.Get(), OnMove);
   glfwSetWindowSizeCallback(mGLFWWindow.Get(), OnResize);
   glfwSetWindowFocusCallback(mGLFWWindow.Get(), OnFocus);
   glfwSetWindowIconifyCallback(mGLFWWindow.Get(), OnMinimize);
   glfwSetFramebufferSizeCallback(mGLFWWindow.Get(), OnResolutionChange);
   glfwSetCursorEnterCallback(mGLFWWindow.Get(), OnHover);
   glfwSetMouseButtonCallback(mGLFWWindow.Get(), OnMouseKey);
   glfwSetScrollCallback(mGLFWWindow.Get(), OnMouseScroll);
   glfwSetCharCallback(mGLFWWindow.Get(), OnTextInput);
   glfwSetDropCallback(mGLFWWindow.Get(), OnFileDrop);

   // Set the user pointer inside GLFWWindow to carry this interface    
   glfwSetWindowUserPointer(mGLFWWindow.Get(), this);

   // Raw mouse motion is closer to the actual motion of the mouse      
   // across a surface. It is not affected by the scaling and           
   // acceleration applied to the motion of the desktop cursor.         
   // That processing is suitable for a cursor while raw motion is      
   // better for controlling for example a 3D camera. Because of this,  
   // raw mouse motion is only provided when the cursor is disabled.    
   if (glfwRawMouseMotionSupported())
      glfwSetInputMode(mGLFWWindow.Get(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

   mNativeWindowHandle = GetNativeWindowPointer(mGLFWWindow.Get());
}

/// Move-construct window                                                     
///   @param other - the window to move                                       
Window::Window(Window&& other) noexcept
   : Unit {Forward<Unit>(other)}
   , mGLFWWindow {Move(other.mGLFWWindow)}
   , mScrollChange {other.mScrollChange} {
   // Make sure the internal user pointer points to new place, too      
   if (mGLFWWindow)
      glfwSetWindowUserPointer(mGLFWWindow.Get(), this);
}

/// Move-copy window                                                          
///   @param other - the window to move                                       
Window& Window::operator = (Window&& other) noexcept {
   Unit::operator = (Forward<Unit>(other));
   mGLFWWindow = Move(other.mGLFWWindow);
   mScrollChange = other.mScrollChange;
   if (mGLFWWindow)
      glfwSetWindowUserPointer(mGLFWWindow.Get(), this);
   return *this;
}

/// Window destruction                                                        
Window::~Window() {
   if (mGLFWWindow) {
      glfwDestroyWindow(mGLFWWindow.Get());
      mGLFWWindow = nullptr;
      mNativeWindowHandle = nullptr;
   }
}

/// Refresh the window component on environment change                        
void Window::Refresh() {
   // Refresh unpinned properties from hierarchy                        
   mOwners.SeekTrait<Traits::Size>(mSize);
   if (mOwners.SeekTrait<Traits::Name>(mTitle))
      glfwSetWindowTitle(mGLFWWindow.Get(), mTitle.Terminate().GetRaw());
}

/// Associate some specific traits of a window                                
/// Many of the window traits require system calls to update, so this takes   
/// care of that                                                              
///   @param verb - association verb                                          
void Window::Associate(Verb& verb) {
   verb.ForEachDeep([&](const Trait& trait) {
      if (trait.TraitIs<Traits::Clipboard>()) {
         // Update system clipboard                                     
         mClipboard = trait.AsCast<Text>().Terminate();
         glfwSetClipboardString(mGLFWWindow.Get(), mClipboard.GetRaw());
      }
   });
}

/// Update the window                                                         
void Window::Update() {
   if (!mGLFWWindow)
      return;

   // Expose the current clipboard - it might be used by other modules, 
   // like UI for example                                               
   mClipboard = Text {glfwGetClipboardString(mGLFWWindow.Get())};


   // Update gradients, even if window is not interactable              
   mMousePosition.Update();
   mMouseScroll.Update();

   if (IsInteractable() && IsMouseOver()) {
      // Handle mouse movement                                          
      double mouseX, mouseY;
      glfwGetCursorPos(mGLFWWindow.Get(), &mouseX, &mouseY);
      mMousePosition.Current() = Vec2 {mouseX, mouseY};

      // Handle mouse scroll                                            
      mMouseScroll.Current() += mScrollChange;
      mScrollChange = {};


      // Check if mouse position has changed, and add specific events   
      auto md = mMousePosition.Delta();
      if (md[0] != 0) {
         // Horizontal mouse movement                                   
         Verbs::Interact interact {Events::MouseMoveHorizontal{md[0]}};
         DoInHierarchy<SeekStyle::DownFromHere>(interact);
      }

      if (md[1] != 0) {
         // Vertical mouse movement                                     
         Verbs::Interact interact {Events::MouseMoveVertical{md[1]}};
         DoInHierarchy<SeekStyle::DownFromHere>(interact);
      }

      if (md[0] != 0 || md[1] != 0) {
         // Any mouse movement                                          
         Verbs::Interact interact {Events::MouseMove{md}};
         DoInHierarchy<SeekStyle::DownFromHere>(interact);
      }

      // Check if mouse scroll has changed, and add specific events     
      auto ms = mMouseScroll.Delta();
      if (ms[0] != 0) {
         // Horizontal scrolling                                        
         Verbs::Interact interact {Events::MouseScrollHorizontal{ms[0]}};
         DoInHierarchy<SeekStyle::DownFromHere>(interact);
      }

      if (ms[1] != 0) {
         // Vertical scrolling                                          
         Verbs::Interact interact {Events::MouseScrollVertical{ms[1]}};
         DoInHierarchy<SeekStyle::DownFromHere>(interact);
      }

      if (ms[0] != 0 || ms[1] != 0) {
         // Any mouse scrolling                                         
         Verbs::Interact interact {Events::MouseScroll{ms}};
         DoInHierarchy<SeekStyle::DownFromHere>(interact);
      }
   }

   if (!mTextInput.IsEmpty()) {
      // Interact using queried text input for the window               
      Verbs::Interact interact {Events::WindowText{Move(mTextInput)}};
      DoInHierarchy<SeekStyle::DownFromHere>(interact);
   }
}

/// Set window size and notify the hierarchy of the change                    
///   @param x - horizontal size                                              
///   @param y - vertical size                                                
void Window::SetSize(int x, int y) {
   mSize = {x, y};
}

/// Check if window is closed                                                 
bool Window::IsClosed() const {
   return glfwGetWindowAttrib(mGLFWWindow.Get(), GLFW_VISIBLE) == GLFW_FALSE;
}

/// Check if window is in focus                                               
bool Window::IsInFocus() const {
   return glfwGetWindowAttrib(mGLFWWindow.Get(), GLFW_FOCUSED) == GLFW_TRUE;
}

/// Check if window is minimized                                              
bool Window::IsMinimized() const {
   return glfwGetWindowAttrib(mGLFWWindow.Get(), GLFW_ICONIFIED) == GLFW_TRUE;
}

/// Check if window is in focus                                               
bool Window::IsMouseOver() const {
   return glfwGetWindowAttrib(mGLFWWindow.Get(), GLFW_HOVERED) == GLFW_TRUE;
}

/// Check if window interacts on inputs                                       
bool Window::IsInteractable() const {
   return !IsClosed() && IsInFocus() && !IsMinimized();
}

/// Get the native window handle                                              
void* Window::GetNativeWindowHandle() const noexcept {
   return mNativeWindowHandle.Get();
}

/// Accumulate text input                                                     
void Window::PushTextInput(const Text& text) {
   mTextInput += text;
}

/// Accumulate mouse scrolling                                                
void Window::AccumulateScroll(const Vec2& offset) noexcept {
   mScrollChange += offset;
}



///                                                                           
///   CALLBACKS                                                               
///                                                                           

/// Get the Langulus window associated with a GLFW window                     
inline auto GetUnit(GLFWwindow* window) {
   return static_cast<Window*>(glfwGetWindowUserPointer(window));
}

/// On window close                                                           
///   @param window - the event's owner                                       
void OnClosed(GLFWwindow* window) {
   auto canvas = GetUnit(window);
   if (!canvas->IsInteractable())
      return;

   glfwHideWindow(window);

   Verbs::Interact interact {
      Events::WindowClose {canvas->GetNativeWindowHandle()}
   };
   canvas->DoInHierarchy<SeekStyle::DownFromHere>(interact);
}

/// On key/mouse press/move                                                   
///   @param window - the event's owner                                       
///   @param key - the pressed/released key                                   
///   @param scancode - i don't know what the fuck this is                    
///   @param action - pressed, released, repeated, etc...                     
///   @param mods - i guess it has something to do with key combinations      
void OnKeyboardKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
   auto canvas = GetUnit(window);
   if (!canvas->IsInteractable())
      return;

   Verbs::Interact interact {};
   EventState state;

   switch (key) {
   case GLFW_KEY_SPACE:
      interact.SetArgument(Keys::Space {state});
      break;
   case GLFW_KEY_APOSTROPHE:
      interact.SetArgument(Keys::Apostrophe {state});
      break;
   case GLFW_KEY_COMMA:
      interact.SetArgument(Keys::Comma {state});
      break;
   case GLFW_KEY_MINUS:
      interact.SetArgument(Keys::Minus {state});
      break;
   case GLFW_KEY_PERIOD:
      interact.SetArgument(Keys::Period {state});
      break;
   case GLFW_KEY_SLASH:
      interact.SetArgument(Keys::Slash {state});
      break;
   case GLFW_KEY_BACKSLASH:
      interact.SetArgument(Keys::Hack {state});
      break;
   case GLFW_KEY_SEMICOLON:
      interact.SetArgument(Keys::Semicolon {state});
      break;
   case GLFW_KEY_LEFT_BRACKET:
      interact.SetArgument(Keys::LeftBracket {state});
      break;
   case GLFW_KEY_RIGHT_BRACKET:
      interact.SetArgument(Keys::RightBracket {state});
      break;
   case GLFW_KEY_GRAVE_ACCENT:
      interact.SetArgument(Keys::Tilde {state});
      break;

   case GLFW_KEY_0:
      interact.SetArgument(Keys::Main0 {state});
      break;
   case GLFW_KEY_1:
      interact.SetArgument(Keys::Main1 {state});
      break;
   case GLFW_KEY_2:
      interact.SetArgument(Keys::Main2 {state});
      break;
   case GLFW_KEY_3:
      interact.SetArgument(Keys::Main3 {state});
      break;
   case GLFW_KEY_4:
      interact.SetArgument(Keys::Main4 {state});
      break;
   case GLFW_KEY_5:
      interact.SetArgument(Keys::Main5 {state});
      break;
   case GLFW_KEY_6:
      interact.SetArgument(Keys::Main6 {state});
      break;
   case GLFW_KEY_7:
      interact.SetArgument(Keys::Main7 {state});
      break;
   case GLFW_KEY_8:
      interact.SetArgument(Keys::Main8 {state});
      break;
   case GLFW_KEY_9:
      interact.SetArgument(Keys::Main9 {state});
      break;

   case GLFW_KEY_A:
      interact.SetArgument(Keys::A {state});
      break;
   case GLFW_KEY_B:
      interact.SetArgument(Keys::B {state});
      break;
   case GLFW_KEY_C:
      interact.SetArgument(Keys::C {state});
      break;
   case GLFW_KEY_D:
      interact.SetArgument(Keys::D {state});
      break;
   case GLFW_KEY_E:
      interact.SetArgument(Keys::E {state});
      break;
   case GLFW_KEY_F:
      interact.SetArgument(Keys::F {state});
      break;
   case GLFW_KEY_G:
      interact.SetArgument(Keys::G {state});
      break;
   case GLFW_KEY_H:
      interact.SetArgument(Keys::H {state});
      break;
   case GLFW_KEY_I:
      interact.SetArgument(Keys::I {state});
      break;
   case GLFW_KEY_J:
      interact.SetArgument(Keys::J {state});
      break;
   case GLFW_KEY_K:
      interact.SetArgument(Keys::K {state});
      break;
   case GLFW_KEY_L:
      interact.SetArgument(Keys::L {state});
      break;
   case GLFW_KEY_M:
      interact.SetArgument(Keys::M {state});
      break;
   case GLFW_KEY_N:
      interact.SetArgument(Keys::N {state});
      break;
   case GLFW_KEY_O:
      interact.SetArgument(Keys::O {state});
      break;
   case GLFW_KEY_P:
      interact.SetArgument(Keys::P {state});
      break;
   case GLFW_KEY_Q:
      interact.SetArgument(Keys::Q {state});
      break;
   case GLFW_KEY_R:
      interact.SetArgument(Keys::R {state});
      break;
   case GLFW_KEY_S:
      interact.SetArgument(Keys::S {state});
      break;
   case GLFW_KEY_T:
      interact.SetArgument(Keys::T {state});
      break;
   case GLFW_KEY_U:
      interact.SetArgument(Keys::U {state});
      break;
   case GLFW_KEY_V:
      interact.SetArgument(Keys::V {state});
      break;
   case GLFW_KEY_W:
      interact.SetArgument(Keys::W {state});
      break;
   case GLFW_KEY_X:
      interact.SetArgument(Keys::X {state});
      break;
   case GLFW_KEY_Y:
      interact.SetArgument(Keys::Y {state});
      break;
   case GLFW_KEY_Z:
      interact.SetArgument(Keys::Z {state});
      break;

   case GLFW_KEY_ESCAPE:
      interact.SetArgument(Keys::Escape {state});
      break;
   case GLFW_KEY_ENTER:
      interact.SetArgument(Keys::Enter {state});
      break;
   case GLFW_KEY_TAB:
      interact.SetArgument(Keys::Tab {state});
      break;
   case GLFW_KEY_BACKSPACE:
      interact.SetArgument(Keys::Back {state});
      break;
   case GLFW_KEY_INSERT:
      interact.SetArgument(Keys::Insert {state});
      break;
   case GLFW_KEY_DELETE:
      interact.SetArgument(Keys::Delete {state});
      break;
   case GLFW_KEY_PAGE_UP:
      interact.SetArgument(Keys::PageUp {state});
      break;
   case GLFW_KEY_PAGE_DOWN:
      interact.SetArgument(Keys::PageDown {state});
      break;
   case GLFW_KEY_HOME:
      interact.SetArgument(Keys::Home {state});
      break;
   case GLFW_KEY_END:
      interact.SetArgument(Keys::End {state});
      break;

   case GLFW_KEY_LEFT:
      interact.SetArgument(Keys::Left {state});
      break;
   case GLFW_KEY_RIGHT:
      interact.SetArgument(Keys::Right {state});
      break;
   case GLFW_KEY_UP:
      interact.SetArgument(Keys::Up {state});
      break;
   case GLFW_KEY_DOWN:
      interact.SetArgument(Keys::Down {state});
      break;

   case GLFW_KEY_CAPS_LOCK:
      interact.SetArgument(Keys::CapsLock {state});
      break;
   case GLFW_KEY_SCROLL_LOCK:
      interact.SetArgument(Keys::ScrollLock {state});
      break;
   case GLFW_KEY_NUM_LOCK:
      interact.SetArgument(Keys::NumLock {state});
      break;
   case GLFW_KEY_PRINT_SCREEN:
      interact.SetArgument(Keys::Print {state});
      break;
   case GLFW_KEY_PAUSE:
      interact.SetArgument(Keys::Pause {state});
      break;

   case GLFW_KEY_F1:
      interact.SetArgument(Keys::F1 {state});
      break;
   case GLFW_KEY_F2:
      interact.SetArgument(Keys::F2 {state});
      break;
   case GLFW_KEY_F3:
      interact.SetArgument(Keys::F3 {state});
      break;
   case GLFW_KEY_F4:
      interact.SetArgument(Keys::F4 {state});
      break;
   case GLFW_KEY_F5:
      interact.SetArgument(Keys::F5 {state});
      break;
   case GLFW_KEY_F6:
      interact.SetArgument(Keys::F6 {state});
      break;
   case GLFW_KEY_F7:
      interact.SetArgument(Keys::F7 {state});
      break;
   case GLFW_KEY_F8:
      interact.SetArgument(Keys::F8 {state});
      break;
   case GLFW_KEY_F9:
      interact.SetArgument(Keys::F9 {state});
      break;
   case GLFW_KEY_F10:
      interact.SetArgument(Keys::F10 {state});
      break;
   case GLFW_KEY_F11:
      interact.SetArgument(Keys::F11 {state});
      break;
   case GLFW_KEY_F12:
      interact.SetArgument(Keys::F12 {state});
      break;
   case GLFW_KEY_F13:
      interact.SetArgument(Keys::F13 {state});
      break;
   case GLFW_KEY_F14:
      interact.SetArgument(Keys::F14 {state});
      break;
   case GLFW_KEY_F15:
      interact.SetArgument(Keys::F15 {state});
      break;
   case GLFW_KEY_F16:
      interact.SetArgument(Keys::F16 {state});
      break;
   case GLFW_KEY_F17:
      interact.SetArgument(Keys::F17 {state});
      break;
   case GLFW_KEY_F18:
      interact.SetArgument(Keys::F18 {state});
      break;
   case GLFW_KEY_F19:
      interact.SetArgument(Keys::F19 {state});
      break;
   case GLFW_KEY_F20:
      interact.SetArgument(Keys::F20 {state});
      break;
   case GLFW_KEY_F21:
      interact.SetArgument(Keys::F21 {state});
      break;
   case GLFW_KEY_F22:
      interact.SetArgument(Keys::F22 {state});
      break;
   case GLFW_KEY_F23:
      interact.SetArgument(Keys::F23 {state});
      break;
   case GLFW_KEY_F24:
      interact.SetArgument(Keys::F24 {state});
      break;

   case GLFW_KEY_KP_0:
      interact.SetArgument(Keys::Numpad0 {state});
      break;
   case GLFW_KEY_KP_1:
      interact.SetArgument(Keys::Numpad1 {state});
      break;
   case GLFW_KEY_KP_2:
      interact.SetArgument(Keys::Numpad2 {state});
      break;
   case GLFW_KEY_KP_3:
      interact.SetArgument(Keys::Numpad3 {state});
      break;
   case GLFW_KEY_KP_4:
      interact.SetArgument(Keys::Numpad4 {state});
      break;
   case GLFW_KEY_KP_5:
      interact.SetArgument(Keys::Numpad5 {state});
      break;
   case GLFW_KEY_KP_6:
      interact.SetArgument(Keys::Numpad6 {state});
      break;
   case GLFW_KEY_KP_7:
      interact.SetArgument(Keys::Numpad7 {state});
      break;
   case GLFW_KEY_KP_8:
      interact.SetArgument(Keys::Numpad8 {state});
      break;
   case GLFW_KEY_KP_9:
      interact.SetArgument(Keys::Numpad9 {state});
      break;

   case GLFW_KEY_KP_EQUAL:
      interact.SetArgument(Keys::NumpadEqual {state});
      break;
   case GLFW_KEY_KP_DECIMAL:
      interact.SetArgument(Keys::NumpadDecimal {state});
      break;
   case GLFW_KEY_KP_DIVIDE:
      interact.SetArgument(Keys::NumpadDivide {state});
      break;
   case GLFW_KEY_KP_MULTIPLY:
      interact.SetArgument(Keys::NumpadMultiply {state});
      break;
   case GLFW_KEY_KP_SUBTRACT:
      interact.SetArgument(Keys::NumpadSubtract {state});
      break;
   case GLFW_KEY_KP_ADD:
      interact.SetArgument(Keys::NumpadAdd {state});
      break;
   case GLFW_KEY_KP_ENTER:
      interact.SetArgument(Keys::NumpadEnter {state});
      break;

   case GLFW_KEY_LEFT_SHIFT:
      interact.SetArgument(Keys::LeftShift {state});
      break;
   case GLFW_KEY_RIGHT_SHIFT:
      interact.SetArgument(Keys::RightShift {state});
      break;

   case GLFW_KEY_LEFT_CONTROL:
      interact.SetArgument(Keys::LeftControl {state});
      break;
   case GLFW_KEY_RIGHT_CONTROL:
      interact.SetArgument(Keys::RightControl {state});
      break;

   case GLFW_KEY_LEFT_ALT:
      interact.SetArgument(Keys::LeftAlt {state});
      break;
   case GLFW_KEY_RIGHT_ALT:
      interact.SetArgument(Keys::RightAlt {state});
      break;
   default:
      return;
   }

   canvas->DoInHierarchy<SeekStyle::DownFromHere>(interact);
}

/// On window moved                                                           
///   @param window - the event's owner                                       
///   @param x - new position (horizontal screen offset in pixels)            
///   @param y - new position (vertical screen offset in pixels)              
void OnMove(GLFWwindow* window, int x, int y) {
   auto canvas = GetUnit(window);
   if (!canvas->IsInteractable())
      return;

   Verbs::Interact interact {Events::WindowMove {Vec2(x, y)}};
   canvas->DoInHierarchy<SeekStyle::DownFromHere>(interact);
}

/// On window resized                                                         
///   @param window - the event's owner                                       
///   @param x - new scale (width in pixels)                                  
///   @param y - new scale (height in pixels)                                 
void OnResize(GLFWwindow* window, int x, int y) {
   auto canvas = GetUnit(window);
   if (!canvas->IsInteractable())
      return;

   // Update hierarchy                                                  
   canvas->SetSize(x, y);

   Verbs::Interact interact {Events::WindowResize {Vec2(x, y)}};
   canvas->DoInHierarchy<SeekStyle::DownFromHere>(interact);
}

/// On window focused or not                                                  
///   @param window - the event's owner                                       
///   @param focused - focused state                                          
void OnFocus(GLFWwindow* window, int focused) {
   auto canvas = GetUnit(window);
   if (canvas->IsClosed())
      return;

   if (focused) {
      Verbs::Interact interact {
         Events::WindowFocus {canvas->GetNativeWindowHandle()}
      };
      canvas->DoInHierarchy<SeekStyle::DownFromHere>(interact);
   }
   else {
      Verbs::Interact interact {
         Events::WindowUnfocus {canvas->GetNativeWindowHandle()}
      };
      canvas->DoInHierarchy<SeekStyle::DownFromHere>(interact);
   }
}

/// On window minimized or not                                                
///   @param window - the event's owner                                       
///   @param iconified - iconification state                                  
void OnMinimize(GLFWwindow* window, int iconified) {
   auto canvas = GetUnit(window);
   if (!canvas->IsInteractable())
      return;

   if (iconified) {
      Verbs::Interact interact {
         Events::WindowMinimize {canvas->GetNativeWindowHandle()}
      };
      canvas->DoInHierarchy<SeekStyle::DownFromHere>(interact);
   }
   else {
      Verbs::Interact interact {
         Events::WindowMaximize {canvas->GetNativeWindowHandle()}
      };
      canvas->DoInHierarchy<SeekStyle::DownFromHere>(interact);
   }
}

/// On window resolution change                                               
///   @param window - the event's owner                                       
///   @param x - new resolution (width in pixels)                             
///   @param y - new resolution (height in pixels)                            
void OnResolutionChange(GLFWwindow* window, int x, int y) {
   auto canvas = GetUnit(window);
   if (canvas->IsClosed())
      return;

   Verbs::Interact interact {Events::WindowResolutionChange {Vec2(x, y)}};
   canvas->DoInHierarchy<SeekStyle::DownFromHere>(interact);
}

/// On mouse enter window                                                     
///   @param window - the event's owner                                       
///   @param entered - zero if leave, one if entered                          
void OnHover(GLFWwindow* window, int entered) {
   auto canvas = GetUnit(window);
   if (!canvas->IsInteractable())
      return;

   if (entered) {
      Verbs::Interact interact {
         Events::WindowMouseHoverIn {canvas->GetNativeWindowHandle()}
      };
      canvas->DoInHierarchy<SeekStyle::DownFromHere>(interact);
   }
   else {
      Verbs::Interact interact {
         Events::WindowMouseHoverOut {canvas->GetNativeWindowHandle()}
      };
      canvas->DoInHierarchy<SeekStyle::DownFromHere>(interact);
   }
}

/// On mouse key                                                              
///   @param window - the event's owner                                       
///   @param button - button that was pressed or realeased                    
///   @param action - the action that the button performed                    
///   @param mods - mods for button combinations                              
void OnMouseKey(GLFWwindow* window, int button, int action, int) {
   auto canvas = GetUnit(window);
   if (!canvas->IsInteractable())
      return;

   Verbs::Interact interact {};
   EventState state;

   switch (button) {
   case GLFW_MOUSE_BUTTON_LEFT:
      interact.SetArgument(Keys::LeftMouse {state});
      break;
   case GLFW_MOUSE_BUTTON_RIGHT:
      interact.SetArgument(Keys::RightMouse {state});
      break;
   case GLFW_MOUSE_BUTTON_MIDDLE:
      interact.SetArgument(Keys::MiddleMouse {state});
      break;
   case GLFW_MOUSE_BUTTON_4:
      interact.SetArgument(Keys::Mouse4 {state});
      break;
   case GLFW_MOUSE_BUTTON_5:
      interact.SetArgument(Keys::Mouse5 {state});
      break;
   case GLFW_MOUSE_BUTTON_6:
      interact.SetArgument(Keys::Mouse6 {state});
      break;
   case GLFW_MOUSE_BUTTON_7:
      interact.SetArgument(Keys::Mouse7 {state});
      break;
   case GLFW_MOUSE_BUTTON_8:
      interact.SetArgument(Keys::Mouse8 {state});
      break;
   default:
      return;
   }

   canvas->DoInHierarchy<SeekStyle::DownFromHere>(interact);
}

/// Returns last written UTF-32 character, affected by mod keys, language     
/// and all localization stuff on your platform                               
///   @param window - the event's owner                                       
///   @param codepoint - UTF-32 code point                                    
void OnTextInput(GLFWwindow* window, unsigned codepoint) {
   auto canvas = GetUnit(window);
   if (!canvas->IsInteractable())
      return;

   //TODO
   //canvas->PushTextInput(Text::FromCodepoint(codepoint));
}

/// On mouse scroll (supports 2D scrolling)                                   
///   @param window - the event's owner                                       
///   @param xoffset - the new mouse x position                               
///   @param yoffset - the new mouse y position                               
void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset) {
   auto canvas = GetUnit(window);
   if (!canvas->IsInteractable())
      return;

   canvas->AccumulateScroll({xoffset, yoffset});
}

/// On file drop to window                                                    
///   @param window - the event's owner                                       
///   @param count - number of dropped files                                  
///   @param paths - deep container with filenames                            
void OnFileDrop(GLFWwindow* window, int count, const char** paths) {
   auto canvas = GetUnit(window);
   if (!canvas->IsInteractable())
      return;

   Events::WindowFileDrop dropped;
   for (int i = 0; i < count; ++i)
      dropped.mPayload << Text {paths[i]};

   Verbs::Interact interact {dropped};
   canvas->DoInHierarchy<SeekStyle::DownFromHere>(interact);
}
