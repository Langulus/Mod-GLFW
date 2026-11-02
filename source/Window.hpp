///                                                                           
/// Langulus::Module::GLFW                                                    
/// Copyright (c) 2015 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Cursor.hpp"
#include <Math/Gradient.hpp>
#include <Math/Vector.hpp>
#include <Entity/Pin.hpp>


namespace GLFW
{
   ///                                                                        
   ///   GLFW window                                                          
   ///                                                                        
   /// Encapsulates an OS window, relays create, move, select verbs to the    
   /// operating system, dispatches interact verbs to hierarchy upon user     
   /// input                                                                  
   struct Window final : Things::Window, ProducedFrom<GLFW::Platform> {
      using CTTI_Abstract  = No;
      using CTTI_Producer  = GLFW::Platform;
      using CTTI_Bases     = Things::Window;
      using CTTI_Ability   = Verbs::Associate;

   private:
      // The window handle (GLFW specific)                              
      Own<GLFWwindow*> mGLFWWindow;
      // Window title                                                   
      Tags::Name::Tag<Pin<Text>> mTitle = "<untitled>";
      // Window size, in pixels                                         
      Tags::Size::Tag<Pin<Scale2>> mSize;
      // Whether or not cursor is enabled                               
      Tags::Cursor::Tag<Own<Cursor*>> mCursor;
      // Whether or not fullscreen is enabled on a specific monitor     
      Tags::Monitor::Tag<Own<Monitor*>> mMonitor;
      // Native window handle, used by other modules, like Vulkan       
      Tags::NativeWindowHandle::Tag<Own<void*>> mNativeWindowHandle;

      // Relative scrolling accumulator                                 
      Vec2 mScrollChange;
      // Mouse position, relative to window                             
      Tags::MousePosition::Tag<Grad2v2> mMousePosition;
      // Mouse scroll                                                   
      Tags::MouseScroll::Tag<Grad2v2> mMouseScroll;
      // Text input accumulator                                         
      Text mTextInput;
      // Clipboard                                                      
      Tags::Clipboard::Tag<Text> mClipboard;

      using CTTI_Members = Members<
         &Window::mSize,
         &Window::mMousePosition,
         &Window::mMouseScroll,
         &Window::mTitle,
         &Window::mCursor,
         &Window::mMonitor,
         &Window::mNativeWindowHandle,
         &Window::mClipboard
      >;

   public:
       Window(GLFW::Platform*, Neat const&);
      ~Window();

      void Associate(Verb&);
      void Refresh();

      bool IsClosed() const;
      bool IsInFocus() const;
      bool IsMouseOver() const;
      bool IsInteractable() const;

      auto GetNativeHandle() const noexcept -> void*;
      auto GetSize() const noexcept -> Scale2;
      bool IsMinimized() const noexcept;

      void Update();
      void SetSize(int, int);
      void PushTextInput(Text const&);
      void AccumulateScroll(Vec2 const&) noexcept;
   };
}