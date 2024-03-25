///                                                                           
/// Langulus::Module::GLFW                                                    
/// Copyright (c) 2015 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Cursor.hpp"
#include <Math/Gradient.hpp>
#include <Math/Vector.hpp>
#include <Entity/Pin.hpp>


namespace GLFW
{

   using Vec2 = Math::Vec2;
   using Grad2v2 = Math::Grad2v2;

   ///                                                                        
   ///   GLFW window                                                          
   ///                                                                        
   /// Encapsulates an OS window, relays create, move, select verbs to the    
   /// operating system, dispatches interact verbs to hierarchy upon user     
   /// input                                                                  
   ///                                                                        
   struct Window final : A::Window, ProducedFrom<GLFW::Platform> {
      LANGULUS(ABSTRACT) false;
      LANGULUS(PRODUCER) GLFW::Platform;
      LANGULUS_BASES(A::Window);
      LANGULUS_VERBS(Verbs::Associate);

   private:
      // The window handle (GLFW specific)                              
      Own<GLFWwindow*> mGLFWWindow;
      // Window title                                                   
      Traits::Name::Tag<Pin<Text>> mTitle = "<untitled>";
      // Window size, in pixels                                         
      Traits::Size::Tag<Pin<Scale2>> mSize;
      // Whether or not cursor is enabled                               
      Traits::Cursor::Tag<Own<Cursor*>> mCursor;
      // Whether or not fullscreen is enabled on a specific monitor     
      Traits::Monitor::Tag<Own<Monitor*>> mMonitor;
      // Native window handle, used by other modules, like Vulkan       
      Traits::NativeWindowHandle::Tag<Own<void*>> mNativeWindowHandle;


      // Relative scrolling accumulator                                 
      Vec2 mScrollChange;
      // Mouse position, relative to window                             
      Traits::MousePosition::Tag<Grad2v2> mMousePosition;
      // Mouse scroll                                                   
      Traits::MouseScroll::Tag<Grad2v2> mMouseScroll;
      // Text input accumulator                                         
      Text mTextInput;
      // Clipboard                                                      
      Traits::Clipboard::Tag<Text> mClipboard;

      LANGULUS_MEMBERS(
         &Window::mSize,
         &Window::mMousePosition,
         &Window::mMouseScroll,
         &Window::mTitle,
         &Window::mCursor,
         &Window::mMonitor,
         &Window::mNativeWindowHandle,
         &Window::mClipboard
      );

   public:
      Window(GLFW::Platform*, const Neat&);
      //Window(Window&&) noexcept;
      ~Window();

      //Window& operator = (Window&&) noexcept;

      void Associate(Verb&);
      void Refresh();

      NOD() bool IsClosed() const;
      NOD() bool IsInFocus() const;
      NOD() bool IsMouseOver() const;
      NOD() bool IsInteractable() const;

      NOD() void* GetNativeHandle() const noexcept;
      NOD() Scale2 GetSize() const noexcept;
      NOD() bool IsMinimized() const noexcept;

      void Update();
      void SetSize(int, int);
      void PushTextInput(const Text&);
      void AccumulateScroll(const Vec2&) noexcept;
   };

} // namespace GLFW