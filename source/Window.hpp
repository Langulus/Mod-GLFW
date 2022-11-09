#pragma once
#include "Common.hpp"
/*
LANGULUS_DECLARE_EVENT(WindowClosed, 
   "Event that occurs when a platform window closes");
LANGULUS_DECLARE_EVENT(WindowFileDrop, 
   "Event that occurs when you drop a file inside a platform window");
LANGULUS_DECLARE_EVENT(WindowMouseEnter, 
   "Event that occurs when mouse enters or leaves a platform window");
LANGULUS_DECLARE_EVENT(WindowResolutionChanged, 
   "Event that occurs when a platform system changes display resolution");
LANGULUS_DECLARE_EVENT(WindowMinimized, 
   "Event that occurs when a platform window is minimized");
LANGULUS_DECLARE_EVENT(WindowFocus, 
   "Event that occurs when a platform window is focused or unfocused");
LANGULUS_DECLARE_EVENT(WindowResized, 
   "Event that occurs when a platform window is resized");
LANGULUS_DECLARE_EVENT(WindowMoved, 
   "Event that occurs when a platform window is moved");
   */

///                                                                           
///   Window component                                                        
///                                                                           
/// Encapsulates a native window, relays create, move, select verbs           
///                                                                           
class Window : public Unit {
   LANGULUS_PRODUCER() Platform;

private:
   // The window handle (GLFW specific)                                 
   Own<GLFWwindow*> mGLFWWindow;
   // If true, means no Vulkan                                          
   bool mOwnDC = true;
   // Relative scrolling accumulator                                    
   vec2 mScrollChange;
   // Text input accumulator                                            
   Text mTextInput;
   // Window title                                                      
   Text mTitle;
   // If title is not owned, we'll be scanning owners for a name        
   bool mOwnTitle = false;

public:
   Window(Platform*, const Any&);
   Window(Window&&) noexcept;
   ~Window();

   Window& operator = (Window&&) noexcept;

   void Refresh() final;
   void BindContext();
   NOD() bool IsInFocus() const;
   NOD() bool IsMinimized() const;
   NOD() bool IsMouseOver() const;
   NOD() bool IsInteractable() const;

   void Initialize();
   void Uninitalize();
   void Update();
   void SetSize(int x, int y);

   inline void PushTextInput(const Text& text) {
      mTextInput += text;
   }

   inline void AccumulateScroll(const vec2& offset) noexcept {
      mScrollChange += offset;
   }
};
