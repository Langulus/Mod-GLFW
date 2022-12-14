///                                                                           
/// Langulus::Module::GLFW                                                    
/// Copyright(C) 2015 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Cursor.hpp"


///                                                                           
///   Native window                                                           
///                                                                           
/// Encapsulates a native window, relays create, move, select verbs to the    
/// operating system, dispatches interact verbs to hierarchy upon user input  
///                                                                           
class Window : public Unit {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) Platform;
   LANGULUS_VERBS(Verbs::Associate);

private:
   // The window handle (GLFW specific)                                 
   Own<GLFWwindow*> mGLFWWindow;
   // Window title                                                      
   Pinnable<Text> mTitle;
   // Window size, in pixels                                            
   Pinnable<Size2> mSize;
   // Whether or not cursor is enabled                                  
   Own<Cursor*> mCursor;
   // Whether or not fullscreen is enabled on a specific monitor        
   Own<Monitor*> mMonitor;
   // Native window handle, used by other modules, like Vulkan          
   Own<void*> mNativeWindowHandle;

   // Relative scrolling accumulator                                    
   Vec2 mScrollChange;
   // Mouse position, relative to window                                
   Grad2v2 mMousePosition;
   // Mouse scroll                                                      
   Grad2v2 mMouseScroll;
   // Text input accumulator                                            
   Text mTextInput;
   // Clipboard                                                         
   Text mClipboard;

   LANGULUS_PROPERTIES_START(Window)
      LANGULUS_PROPERTY_TRAIT(mSize, Size),
      LANGULUS_PROPERTY_TRAIT(mMousePosition, MousePosition),
      LANGULUS_PROPERTY_TRAIT(mMouseScroll, MouseScroll),
      LANGULUS_PROPERTY_TRAIT(mTitle, Name),
      LANGULUS_PROPERTY_TRAIT(mCursor, Cursor),
      LANGULUS_PROPERTY_TRAIT(mMonitor, Monitor),
      LANGULUS_PROPERTY_TRAIT(mNativeWindowHandle, NativeWindowHandle),
      LANGULUS_PROPERTY_TRAIT(mClipboard, Clipboard)
   LANGULUS_PROPERTIES_END();

public:
   Window(const Any&);
   Window(Window&&) noexcept;
   ~Window();

   Window& operator = (Window&&) noexcept;

   void Associate(Verb&);

   void Refresh() final;

   NOD() bool IsClosed() const;
   NOD() bool IsInFocus() const;
   NOD() bool IsMinimized() const;
   NOD() bool IsMouseOver() const;
   NOD() bool IsInteractable() const;
   NOD() void* GetNativeWindowHandle() const noexcept;

   void Update();
   void SetSize(int, int);
   void PushTextInput(const Text&);
   void AccumulateScroll(const Vec2&) noexcept;
};
