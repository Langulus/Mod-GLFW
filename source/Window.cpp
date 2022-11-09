#include "Window.hpp"
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

/// Callback predefinitions for various window events                         
void OnClosed(GLFWwindow*);
void OnKeyboardKey(GLFWwindow*, int key, int scancode, int action, UNUSED() int mods);
void OnMove(GLFWwindow*, int x, int y);
void OnResize(GLFWwindow*, int x, int y);
void OnFocus(GLFWwindow*, int focused);
void OnMinimize(GLFWwindow*, int iconified);
void OnResolutionChange(GLFWwindow*, int x, int y);
void OnHover(GLFWwindow*, int entered);
void OnMouseKey(GLFWwindow*, int button, int action, UNUSED() int mods);
void OnMouseScroll(GLFWwindow*, double xoffset, double yoffset);
void OnTextInput(GLFWwindow*, unsigned int codepoint);
void OnFileDrop(GLFWwindow*, int count, const char** paths);

/// Window construction                                                       
///   @param producer - the producer of the window                            
///   @param descriptor - window descriptor                                   
Window::Window(Platform* producer, const Any& descriptor)
   : Unit {MetaOf<Window>()} { }

/// Move-construct window                                                     
///   @param other - the window to move                                       
Window::Window(Window&& other) noexcept
   : Unit {Forward<Unit>(other)}
   , mGLFWWindow {Move(other.mGLFWWindow)}
   , mOwnDC {other.mOwnDC}
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
   mOwnDC = other.mOwnDC;
   mScrollChange = other.mScrollChange;
   if (mGLFWWindow)
      glfwSetWindowUserPointer(mGLFWWindow.Get(), this);
   return *this;
}

/// Window destruction                                                        
Window::~Window() {
   Uninitalize();
}

/// Initialize the window                                                     
void Window::Initialize() {
   if (mGLFWWindow)
      return;

   // Make it visible                                                   
   glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
   glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
   mOwnDC = false;

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

   // Set the user pointer inside mWindow to carry this component       
   glfwSetWindowUserPointer(mGLFWWindow.Get(), this);

   if (glfwRawMouseMotionSupported())
      glfwSetInputMode(mGLFWWindow.Get(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

   // Activate rendering context, so that other modules react           
   if (mOwnDC)
      glfwMakeContextCurrent(mGLFWWindow.Get());

   AWindow::mNativeWindowHandle = GetNativeWindowPointer(mGLFWWindow.Get());
   AWindow::mRenderer = GetOwners()[0]->CreateUnit<ARenderer>(this);
}

/// Uninitialize the window                                                   
void Window::Uninitalize() {
   if (mGLFWWindow) {
      glfwDestroyWindow(mGLFWWindow.Get());
      mGLFWWindow = nullptr;
      AWindow::mRenderer.Reset();
      AWindow::mNativeWindowHandle.Reset();
   }
}

/// Bind context                                                              
void Window::BindContext() {
   if (mOwnDC) {
      glfwMakeContextCurrent(mGLFWWindow.Get());
      glfwSwapBuffers(mGLFWWindow.Get());
   }
}

/// Refresh the window component, initializing it if needed                   
void Window::Refresh() {
   Initialize();

   if (mGLFWWindow && !mOwnTitle) {
      // Attempt extracting title from owners                           
      for (auto owner : GetOwners()) {
         auto name = owner->GetName();
         if (!name.IsEmpty()) {
            // Break on first find                                      
            mTitle = Move(name);
            break;
         }
      }

      glfwSetWindowTitle(mGLFWWindow.Get(), mTitle.Terminate().GetRaw());
   }
}

/// Update the window                                                         
void Window::Update() {
   if (!mGLFWWindow)
      return;

   mMousePosition.Update();
   mMouseScroll.Update();

   if (IsInteractable() && IsMouseOver()) {
      // Handle mouse movement                                          
      double mouseX, mouseY;
      glfwGetCursorPos(mGLFWWindow.Get(), &mouseX, &mouseY);
      mMousePosition.Current() = vec2(real(mouseX), real(mouseY));

      // Check if mouse position has changed, and add specific events   
      auto md = mMousePosition.Delta();
      if (!pcNear(md[0], real(0))) {
         // Horizontal mouse movement                                   
         auto interact = Verb::From<Verbs::Interact>(
            {}, Any::Wrap(Keys::MouseMoveHorizontal::ID, md[0]));
         DoInHierarchy(interact, SeekStyle::DownFromHere);
      }

      if (!pcNear(md[1], real(0))) {
         // Vertical mouse movement                                     
         auto interact = Verb::From<Verbs::Interact>(
            {}, Any::Wrap(Keys::MouseMoveVertical::ID, md[1]));
         DoInHierarchy(interact, SeekStyle::DownFromHere);
      }

      // Handle mouse scroll                                            
      mMouseScroll.Current() += mScrollChange;
      mScrollChange = {};

      // Check if mouse scroll has changed, and add specific events     
      auto ms = mMouseScroll.Delta();
      if (!pcNear(ms[0], real(0))) {
         // Horizontal scrolling                                        
         auto interact = Verb::From<Verbs::Interact>(
            {}, Any::Wrap(Keys::MouseScrollHorizontal::ID, real(ms[0])));
         DoInHierarchy(interact, SeekStyle::DownFromHere);
      }

      if (!pcNear<real>(ms[1], 0)) {
         // Vertical scrolling                                          
         auto interact = Verb::From<Verbs::Interact>(
            {}, Any::Wrap(Keys::MouseScrollVertical::ID, real(ms[1])));
         DoInHierarchy(interact, SeekStyle::DownFromHere);
      }
   }

   if (!mTextInput.IsEmpty()) {
      // Interact using the text inputs                                 
      //auto interact = uvInteract({}, PCT(buffer, count));
      //canvas->DoInHierarchy(interact);mTextInput
      mTextInput.Clear();
   }
}

/// Resize the window                                                         
///   @param x - horizontal size                                              
///   @param y - vertical size                                                
void Window::SetSize(int x, int y) {
   const vec2 newSize {x, y};
   if (AWindow::mSize != newSize && AWindow::mRenderer) {
      AWindow::mRenderer->Resize(newSize);
      AWindow::mSize = newSize;
   }
}

/// Check if window is in focus                                               
bool Window::IsInFocus() const {
   return glfwGetWindowAttrib(mGLFWWindow.Get(), GLFW_FOCUSED) == GLFW_TRUE;
}

/// Check if window is in focus                                               
bool Window::IsMinimized() const {
   return glfwGetWindowAttrib(mGLFWWindow.Get(), GLFW_ICONIFIED) == GLFW_TRUE;
}

/// Check if window is in focus                                               
bool Window::IsMouseOver() const {
   return glfwGetWindowAttrib(mGLFWWindow.Get(), GLFW_HOVERED) == GLFW_TRUE;
}

/// Check if window interacts with piception                                  
bool Window::IsInteractable() const {
   return IsClassEnabled() && IsInFocus() && !IsMinimized();
}

void Window::PushTextInput(const Text& text) {
   mTextInput += text;
}

void Window::AccumulateScroll(const vec2& offset) noexcept {
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
   canvas->ClassDisable();
   auto interact = Verb::From<Verbs::Interact>(
      {}, Events::WindowClosed::ID);
   canvas->DoInHierarchy(interact, SeekStyle::DownFromHere);
}

/// On key/mouse press/move																	
///	@param window - the event's owner													
///	@param key - the pressed/released key												
///	@param scancode - i don't know what the fuck this is							
///	@param action - pressed, released, repeated, etc...							
///	@param mods - i guess it has something to do with key combinations		
void OnKeyboardKey(GLFWwindow* window, int key, int, int action, int) {
   auto canvas = GetUnit(window);
   if (!canvas->IsInteractable())
      return;
   if (action != GLFW_PRESS && action != GLFW_RELEASE)
      return;

   auto interact = Verb::From<Verbs::Interact>(
      {}, PCFW::GLFWKeyboardTable[key]);
   if (action == GLFW_RELEASE)
      interact.Invert();
   canvas->DoInHierarchy(interact, SeekStyle::DownFromHere);
}

/// On window moved																				
///	@param window - the event's owner													
///	@param x - new position (horizontal screen offset in pixels)				
///	@param y - new position (vertical screen offset in pixels)					
void OnMove(GLFWwindow* window, int x, int y) {
   auto canvas = GetUnit(window);
   if (!canvas->IsInteractable())
      return;

   auto interact = Verb::From<Verbs::Interact>(
      {}, Any::Wrap(Events::WindowMoved::ID, vec2(real(x), real(y))));
   canvas->DoInHierarchy(interact, SeekStyle::DownFromHere);
}

/// On window resized																			
///	@param window - the event's owner													
///	@param x - new scale (width in pixels)												
///	@param y - new scale (height in pixels)											
void OnResize(GLFWwindow* window, int x, int y) {
   auto canvas = GetUnit(window);
   if (!canvas->IsInteractable())
      return;

   canvas->SetSize(x, y);
   auto interact = Verb::From<Verbs::Interact>(
      {}, Any::Wrap(Events::WindowResized::ID, vec2(real(x), real(y))));
   canvas->DoInHierarchy(interact, SeekStyle::DownFromHere);
}

/// On window focused or not																	
///	@param window - the event's owner													
///	@param focused - focused state														
void OnFocus(GLFWwindow* window, int focused) {
   auto canvas = GetUnit(window);
   if (canvas->IsClassDisabled())
      return;

   auto interact = Verb::From<Verbs::Interact>(
      {}, Events::WindowFocus::ID);
   if (!focused)
      interact.Invert();
   canvas->DoInHierarchy(interact, SeekStyle::DownFromHere);
}

/// On window minimized or not																
///	@param window - the event's owner													
///	@param iconified - iconification state												
void OnMinimize(GLFWwindow* window, int iconified) {
   auto canvas = GetUnit(window);
   if (!canvas->IsInteractable())
      return;

   auto interact = Verb::From<Verbs::Interact>(
      {}, Events::WindowMinimized::ID);
   if (!iconified)
      interact.Invert();
   canvas->DoInHierarchy(interact, SeekStyle::DownFromHere);
}

/// On window resolution change																
///	@param window - the event's owner													
///	@param x - new resolution (width in pixels)										
///	@param y - new resolution (height in pixels)										
void OnResolutionChange(GLFWwindow* window, int x, int y) {
   auto canvas = GetUnit(window);
   if (canvas->IsClassDisabled())
      return;

   auto interact = Verb::From<Verbs::Interact>(
      {}, Any::Wrap(Events::WindowResolutionChanged::ID, vec2(real(x), real(y))));
   canvas->DoInHierarchy(interact, SeekStyle::DownFromHere);
}

/// On mouse enter window																		
///	@param window - the event's owner													
///	@param entered - zero if leave, one if entered									
void OnHover(GLFWwindow* window, int entered) {
   auto canvas = GetUnit(window);
   if (!canvas->IsInteractable())
      return;

   auto interact = Verb::From<Verbs::Interact>(
      {}, Events::WindowMouseEnter::ID);
   if (!entered)
      interact.Invert();
   canvas->DoInHierarchy(interact, SeekStyle::DownFromHere);
}

/// On mouse key																					
///	@param window - the event's owner													
///	@param button - button that was pressed or realeased							
///	@param action - the action that the button performed							
///	@param mods - mods for button combinations										
void OnMouseKey(GLFWwindow* window, int button, int action, int) {
   auto canvas = GetUnit(window);
   if (!canvas->IsInteractable())
      return;
   if (action != GLFW_PRESS && action != GLFW_RELEASE)
      return;

   auto interact = Verb::From<Verbs::Interact>(
      {}, PCFW::GLFWMouseTable[button]);
   if (action == GLFW_RELEASE)
      interact.Invert();
   canvas->DoInHierarchy(interact, SeekStyle::DownFromHere);
}

/// Returns last written UTF-32 character, affected by mod keys, language		
/// and all localization stuff on your platform.										
///	@param window - the event's owner													
///	@param codepoint - UTF-32 code point												
void OnTextInput(GLFWwindow* window, uint codepoint) {
   auto canvas = GetUnit(window);
   if (!canvas->IsInteractable())
      return;

   canvas->PushTextInput(Text::FromCodepoint(codepoint));
}

/// On mouse scroll (supports 2D scrolling)												
///	@param window - the event's owner													
///	@param xoffset - the new mouse x position											
///	@param yoffset - the new mouse y position											
void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset) {
   auto canvas = GetUnit(window);
   if (!canvas->IsInteractable())
      return;

   canvas->AccumulateScroll({real(xoffset), real(yoffset)});
}

/// On file drop to window																	
///	@param window - the event's owner												
///	@param count - number of dropped files											
///	@param paths - deep container with filenames									
void OnFileDrop(GLFWwindow* window, int count, const char** paths) {
   auto canvas = GetUnit(window);
   if (!canvas->IsInteractable())
      return;

   Any package;
   package << Any::Wrap(Events::WindowFileDrop::ID);
   for (int i = 0; i < count; ++i)
      package << Text(paths[i]);

   auto interact = Verb::From<Verbs::Interact>({}, &package);
   canvas->DoInHierarchy(interact, SeekStyle::DownFromHere);
}
