///                                                                           
/// Langulus::Module::GLFW                                                    
/// Copyright(C) 2015 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Common.hpp"

namespace Langulus
{

   ///                                                                        
   ///   Event state                                                          
   ///                                                                        
   struct EventState {
      LANGULUS(POD) true;
      LANGULUS(NULLIFIABLE) true;

      enum Enum {
         // Default event state - a point event, that does not have a   
         // beginning or end, it happens once, immediately              
         Point = 0,

         // The beginning of an event, such as a key press              
         Begin = 1,

         // The end of an event, such as key release                    
         End = 2,
      };

      using Type = ::std::underlying_type_t<Enum>;

      Type mState {Point};

   public:
      constexpr EventState() noexcept = default;
      constexpr EventState(const Type&) noexcept;

      explicit constexpr operator bool() const noexcept;
      constexpr bool operator == (const EventState&) const noexcept = default;

      NOD() constexpr EventState operator + (const EventState&) const noexcept;
      NOD() constexpr EventState operator - (const EventState&) const noexcept;
      constexpr EventState& operator += (const EventState&) noexcept;
      constexpr EventState& operator -= (const EventState&) noexcept;

      NOD() constexpr bool operator & (const EventState&) const noexcept;
      NOD() constexpr bool operator % (const EventState&) const noexcept;

      NOD() constexpr bool IsPoint() const noexcept;
      NOD() constexpr bool IsBegin() const noexcept;
      NOD() constexpr bool IsEnd() const noexcept;

      constexpr void Reset() noexcept;
   };


   ///                                                                        
   ///   An event                                                             
   ///                                                                        
   ///   Simply a named container, that has some of the conventional data     
   /// associated with events, like event type, state and timestamp moved     
   /// to the stack, for less allocations, and faster runtime retrieval       
   ///   Events are often used as arguments for Verbs::Interact               
   ///                                                                        
   struct Event {
      // Event type                                                     
      DMeta mType {};
      // Event state                                                    
      EventState mState;
      // Event timestamp                                                
      TimePoint mTimestamp;
      // Payload, for additional data                                   
      Any mPayload;

      template<CT::Data... PAYLOAD>
      Event(DMeta, EventState, PAYLOAD&&...);
   };

   namespace CT
   {

      /// Concept for detecting any Event type specialization                 
      template<class... T>
      concept Event = (DerivedFrom<T, ::Langulus::Event> && ...);

   } // namespace Langulus::CT


   /// Define an event specialization in ::Langulus::Events namespace         
   ///   @param EVENT - name of the event type                                
   ///   @param INFOSTRING - info string for the event                        
   #define LANGULUS_DEFINE_EVENT(EVENT, INFOSTRING) \
      namespace Events { \
         struct EVENT : Event { \
            LANGULUS(INFO) INFOSTRING; \
            LANGULUS_BASES(Event); \
            template<CT::Data... ARGUMENTS> \
            EVENT(ARGUMENTS&&... payload) \
               : Event { \
                  MetaData::Of<EVENT>(), {}, \
                  Forward<ARGUMENTS>(payload)... \
               } {} \
         }; \
      }

   /// Define an event specialization in ::Langulus::Keys namespace           
   ///   @param EVENT - name of the key type                                  
   ///   @param INFOSTRING - info string for the key                          
   #define LANGULUS_DEFINE_KEY(EVENT, INFOSTRING) \
      namespace Keys { \
         struct EVENT : Event { \
            LANGULUS(INFO) INFOSTRING; \
            LANGULUS_BASES(Event); \
            template<CT::Data... ARGUMENTS> \
            EVENT(EventState state, ARGUMENTS&&... payload) \
               : Event { \
                  MetaData::Of<EVENT>(), state, \
                  Forward<ARGUMENTS>(payload)... \
               } {} \
         }; \
      }


   LANGULUS_DEFINE_EVENT(WindowClose,
      "An event that occurs when native window closes")
   LANGULUS_DEFINE_EVENT(WindowFileDrop,
      "An event that occurs when a file is dropped on a native window")
   LANGULUS_DEFINE_EVENT(WindowMouseHoverIn,
      "An event that occurs when mouse hovers in a native window")
   LANGULUS_DEFINE_EVENT(WindowMouseHoverOut,
      "An event that occurs when mouse hovers out of a native window")
   LANGULUS_DEFINE_EVENT(WindowResolutionChange,
      "An event that occurs when native system resolution changes")
   LANGULUS_DEFINE_EVENT(WindowMinimize,
      "An event that occurs when native window is minimized")
   LANGULUS_DEFINE_EVENT(WindowMaximize,
      "An event that occurs when native window is maximized")
   LANGULUS_DEFINE_EVENT(WindowFocus,
      "An event that occurs when native window gains focus")
   LANGULUS_DEFINE_EVENT(WindowUnfocus,
      "An event that occurs when native window loses focus")
   LANGULUS_DEFINE_EVENT(WindowResize,
      "An event that occurs when native window is resized")
   LANGULUS_DEFINE_EVENT(WindowMove,
      "An event that occurs when native window is moved")
   LANGULUS_DEFINE_EVENT(WindowText,
      "An event that occurs when a text input is queried for a window")

   LANGULUS_DEFINE_EVENT(MouseMoveHorizontal,
      "An event that occurs when mouse moves horizontally inside the window")
   LANGULUS_DEFINE_EVENT(MouseMoveVertical,
      "An event that occurs when mouse moves vertically inside the window")
   LANGULUS_DEFINE_EVENT(MouseMove,
      "An event that occurs when mouse moves in any direction inside the window")
   LANGULUS_DEFINE_EVENT(MouseScrollHorizontal,
      "An event that occurs when mouse scrolls horizontally inside the window")
   LANGULUS_DEFINE_EVENT(MouseScrollVertical,
      "An event that occurs when mouse scrolls vertically inside the window")
   LANGULUS_DEFINE_EVENT(MouseScroll,
      "An event that occurs when mouse scrolls in any direction inside the window")

   LANGULUS_DEFINE_KEY(Space,
      "The event of pressing/releasing the space bar")
   LANGULUS_DEFINE_KEY(Apostrophe,
      "The event of pressing/releasing the ' (apostrophe) key")
   LANGULUS_DEFINE_KEY(Comma,
      "The event of pressing/releasing the , (comma) key")
   LANGULUS_DEFINE_KEY(Minus,
      "The event of pressing/releasing the - (minus) key")
   LANGULUS_DEFINE_KEY(Period,
      "The event of pressing/releasing the . (period) key")
   LANGULUS_DEFINE_KEY(Slash,
      "The event of pressing/releasing the / (forward slash) key")
   LANGULUS_DEFINE_KEY(Hack,
      "The event of pressing/releasing the \\ (backward slash) key")
   LANGULUS_DEFINE_KEY(Semicolon,
      "The event of pressing/releasing the ; (semicolon) key")
   LANGULUS_DEFINE_KEY(LeftBracket,
      "The event of pressing/releasing the [ (left bracket) key")
   LANGULUS_DEFINE_KEY(RightBracket,
      "The event of pressing/releasing the ] (right bracket) key")
   LANGULUS_DEFINE_KEY(Tilde,
      "The event of pressing/releasing the ~ (tilde) key")

   LANGULUS_DEFINE_KEY(Main0,
      "The event of pressing/releasing the main keyboard 0 (zero) key")
   LANGULUS_DEFINE_KEY(Main1,
      "The event of pressing/releasing the main keyboard 1 (one) key")
   LANGULUS_DEFINE_KEY(Main2,
      "The event of pressing/releasing the main keyboard 2 (two) key")
   LANGULUS_DEFINE_KEY(Main3,
      "The event of pressing/releasing the main keyboard 3 (three) key")
   LANGULUS_DEFINE_KEY(Main4,
      "The event of pressing/releasing the main keyboard 4 (four) key")
   LANGULUS_DEFINE_KEY(Main5,
      "The event of pressing/releasing the main keyboard 5 (five) key")
   LANGULUS_DEFINE_KEY(Main6,
      "The event of pressing/releasing the main keyboard 6 (six) key")
   LANGULUS_DEFINE_KEY(Main7,
      "The event of pressing/releasing the main keyboard 7 (seven) key")
   LANGULUS_DEFINE_KEY(Main8,
      "The event of pressing/releasing the main keyboard 8 (eight) key")
   LANGULUS_DEFINE_KEY(Main9,
      "The event of pressing/releasing the main keyboard 9 (nine) key")

   LANGULUS_DEFINE_KEY(A,
      "The event of pressing/releasing the A key")
   LANGULUS_DEFINE_KEY(B,
      "The event of pressing/releasing the B key")
   LANGULUS_DEFINE_KEY(C,
      "The event of pressing/releasing the C key")
   LANGULUS_DEFINE_KEY(D,
      "The event of pressing/releasing the D key")
   LANGULUS_DEFINE_KEY(E,
      "The event of pressing/releasing the E key")
   LANGULUS_DEFINE_KEY(F,
      "The event of pressing/releasing the F key")
   LANGULUS_DEFINE_KEY(G,
      "The event of pressing/releasing the G key")
   LANGULUS_DEFINE_KEY(H,
      "The event of pressing/releasing the H key")
   LANGULUS_DEFINE_KEY(I,
      "The event of pressing/releasing the I key")
   LANGULUS_DEFINE_KEY(J,
      "The event of pressing/releasing the J key")
   LANGULUS_DEFINE_KEY(K,
      "The event of pressing/releasing the K key")
   LANGULUS_DEFINE_KEY(L,
      "The event of pressing/releasing the L key")
   LANGULUS_DEFINE_KEY(M,
      "The event of pressing/releasing the M key")
   LANGULUS_DEFINE_KEY(N,
      "The event of pressing/releasing the N key")
   LANGULUS_DEFINE_KEY(O,
      "The event of pressing/releasing the O key")
   LANGULUS_DEFINE_KEY(P,
      "The event of pressing/releasing the P key")
   LANGULUS_DEFINE_KEY(Q,
      "The event of pressing/releasing the Q key")
   LANGULUS_DEFINE_KEY(R,
      "The event of pressing/releasing the R key")
   LANGULUS_DEFINE_KEY(S,
      "The event of pressing/releasing the S key")
   LANGULUS_DEFINE_KEY(T,
      "The event of pressing/releasing the T key")
   LANGULUS_DEFINE_KEY(U,
      "The event of pressing/releasing the U key")
   LANGULUS_DEFINE_KEY(V,
      "The event of pressing/releasing the V key")
   LANGULUS_DEFINE_KEY(W,
      "The event of pressing/releasing the W key")
   LANGULUS_DEFINE_KEY(X,
      "The event of pressing/releasing the X key")
   LANGULUS_DEFINE_KEY(Y,
      "The event of pressing/releasing the Y key")
   LANGULUS_DEFINE_KEY(Z,
      "The event of pressing/releasing the Z key")

   LANGULUS_DEFINE_KEY(Escape,
      "The event of pressing/releasing the escape key")
   LANGULUS_DEFINE_KEY(Enter,
      "The event of pressing/releasing the enter (return) key")
   LANGULUS_DEFINE_KEY(Tab,
      "The event of pressing/releasing the tab key")
   LANGULUS_DEFINE_KEY(Back,
      "The event of pressing/releasing the backspace key")
   LANGULUS_DEFINE_KEY(Insert,
      "The event of pressing/releasing the insert key")
   LANGULUS_DEFINE_KEY(Delete,
      "The event of pressing/releasing the delete key")
   LANGULUS_DEFINE_KEY(PageUp,
      "The event of pressing/releasing the page-up key")
   LANGULUS_DEFINE_KEY(PageDown,
      "The event of pressing/releasing the page-down key")
   LANGULUS_DEFINE_KEY(Home,
      "The event of pressing/releasing the home key")
   LANGULUS_DEFINE_KEY(End,
      "The event of pressing/releasing the end key")

   LANGULUS_DEFINE_KEY(Left,
      "The event of pressing/releasing the left arrow key")
   LANGULUS_DEFINE_KEY(Right,
      "The event of pressing/releasing the right arrow key")
   LANGULUS_DEFINE_KEY(Up,
      "The event of pressing/releasing the up arrow key")
   LANGULUS_DEFINE_KEY(Down,
      "The event of pressing/releasing the down arrow key")

   LANGULUS_DEFINE_KEY(CapsLock,
      "The event of pressing/releasing the caps-lock key")
   LANGULUS_DEFINE_KEY(ScrollLock,
      "The event of pressing/releasing the scroll-lock key")
   LANGULUS_DEFINE_KEY(NumLock,
      "The event of pressing/releasing the num-lock key")
   LANGULUS_DEFINE_KEY(Print,
      "The event of pressing/releasing the print key")
   LANGULUS_DEFINE_KEY(Pause,
      "The event of pressing/releasing the pause key")

   LANGULUS_DEFINE_KEY(F1,
      "The event of pressing/releasing the F1 (function 1) key")
   LANGULUS_DEFINE_KEY(F2,
      "The event of pressing/releasing the F2 (function 2) key")
   LANGULUS_DEFINE_KEY(F3,
      "The event of pressing/releasing the F3 (function 3) key")
   LANGULUS_DEFINE_KEY(F4,
      "The event of pressing/releasing the F4 (function 4) key")
   LANGULUS_DEFINE_KEY(F5,
      "The event of pressing/releasing the F5 (function 5) key")
   LANGULUS_DEFINE_KEY(F6,
      "The event of pressing/releasing the F6 (function 6) key")
   LANGULUS_DEFINE_KEY(F7,
      "The event of pressing/releasing the F7 (function 7) key")
   LANGULUS_DEFINE_KEY(F8,
      "The event of pressing/releasing the F8 (function 8) key")
   LANGULUS_DEFINE_KEY(F9,
      "The event of pressing/releasing the F9 (function 9) key")
   LANGULUS_DEFINE_KEY(F10,
      "The event of pressing/releasing the F10 (function 10) key")
   LANGULUS_DEFINE_KEY(F11,
      "The event of pressing/releasing the F11 (function 11) key")
   LANGULUS_DEFINE_KEY(F12,
      "The event of pressing/releasing the F12 (function 12) key")
   LANGULUS_DEFINE_KEY(F13,
      "The event of pressing/releasing the F13 (function 13) key")
   LANGULUS_DEFINE_KEY(F14,
      "The event of pressing/releasing the F14 (function 14) key")
   LANGULUS_DEFINE_KEY(F15,
      "The event of pressing/releasing the F15 (function 15) key")
   LANGULUS_DEFINE_KEY(F16,
      "The event of pressing/releasing the F16 (function 16) key")
   LANGULUS_DEFINE_KEY(F17,
      "The event of pressing/releasing the F17 (function 17) key")
   LANGULUS_DEFINE_KEY(F18,
      "The event of pressing/releasing the F18 (function 18) key")
   LANGULUS_DEFINE_KEY(F19,
      "The event of pressing/releasing the F19 (function 19) key")
   LANGULUS_DEFINE_KEY(F20,
      "The event of pressing/releasing the F20 (function 20) key")
   LANGULUS_DEFINE_KEY(F21,
      "The event of pressing/releasing the F21 (function 21) key")
   LANGULUS_DEFINE_KEY(F22,
      "The event of pressing/releasing the F22 (function 22) key")
   LANGULUS_DEFINE_KEY(F23,
      "The event of pressing/releasing the F23 (function 23) key")
   LANGULUS_DEFINE_KEY(F24,
      "The event of pressing/releasing the F24 (function 24) key")

   LANGULUS_DEFINE_KEY(Numpad0,
      "The event of pressing/releasing the numpad 0 key")
   LANGULUS_DEFINE_KEY(Numpad1,
      "The event of pressing/releasing the numpad 1 key")
   LANGULUS_DEFINE_KEY(Numpad2,
      "The event of pressing/releasing the numpad 2 key")
   LANGULUS_DEFINE_KEY(Numpad3,
      "The event of pressing/releasing the numpad 3 key")
   LANGULUS_DEFINE_KEY(Numpad4,
      "The event of pressing/releasing the numpad 4 key")
   LANGULUS_DEFINE_KEY(Numpad5,
      "The event of pressing/releasing the numpad 5 key")
   LANGULUS_DEFINE_KEY(Numpad6,
      "The event of pressing/releasing the numpad 6 key")
   LANGULUS_DEFINE_KEY(Numpad7,
      "The event of pressing/releasing the numpad 7 key")
   LANGULUS_DEFINE_KEY(Numpad8,
      "The event of pressing/releasing the numpad 8 key")
   LANGULUS_DEFINE_KEY(Numpad9,
      "The event of pressing/releasing the numpad 9 key")

   LANGULUS_DEFINE_KEY(NumpadEqual,
      "The event of pressing/releasing the numpad = (equal) key")
   LANGULUS_DEFINE_KEY(NumpadDecimal,
      "The event of pressing/releasing the numpad . (decimal) key")
   LANGULUS_DEFINE_KEY(NumpadDivide,
      "The event of pressing/releasing the numpad / (divide) key")
   LANGULUS_DEFINE_KEY(NumpadMultiply,
      "The event of pressing/releasing the numpad * (multiply) key")
   LANGULUS_DEFINE_KEY(NumpadSubtract,
      "The event of pressing/releasing the numpad - (subtract) key")
   LANGULUS_DEFINE_KEY(NumpadAdd,
      "The event of pressing/releasing the numpad + (add) key")
   LANGULUS_DEFINE_KEY(NumpadEnter,
      "The event of pressing/releasing the numpad enter key")

   LANGULUS_DEFINE_KEY(LeftShift,
      "The event of pressing/releasing the left shift key")
   LANGULUS_DEFINE_KEY(RightShift,
      "The event of pressing/releasing the right shift key")

   LANGULUS_DEFINE_KEY(LeftControl,
      "The event of pressing/releasing the left control key")
   LANGULUS_DEFINE_KEY(RightControl,
      "The event of pressing/releasing the right control key")

   LANGULUS_DEFINE_KEY(LeftAlt,
      "The event of pressing/releasing the left alt key")
   LANGULUS_DEFINE_KEY(RightAlt,
      "The event of pressing/releasing the right alt key")

   LANGULUS_DEFINE_KEY(LeftMouse,
      "The event of pressing/releasing the left mouse key")
   LANGULUS_DEFINE_KEY(MiddleMouse,
      "The event of pressing/releasing the middle mouse key")
   LANGULUS_DEFINE_KEY(RightMouse,
      "The event of pressing/releasing the right mouse key")
   LANGULUS_DEFINE_KEY(Mouse4,
      "The event of pressing/releasing the mouse #4 key")
   LANGULUS_DEFINE_KEY(Mouse5,
      "The event of pressing/releasing the mouse #5 key")
   LANGULUS_DEFINE_KEY(Mouse6,
      "The event of pressing/releasing the mouse #6 key")
   LANGULUS_DEFINE_KEY(Mouse7,
      "The event of pressing/releasing the mouse #7 key")
   LANGULUS_DEFINE_KEY(Mouse8,
      "The event of pressing/releasing the mouse #8 key")

   LANGULUS_DEFINE_KEY(Joystick1,
      "The event of pressing/releasing the joystick #1 key")
   LANGULUS_DEFINE_KEY(Joystick2,
      "The event of pressing/releasing the joystick #2 key")
   LANGULUS_DEFINE_KEY(Joystick3,
      "The event of pressing/releasing the joystick #3 key")
   LANGULUS_DEFINE_KEY(Joystick4,
      "The event of pressing/releasing the joystick #4 key")
   LANGULUS_DEFINE_KEY(Joystick5,
      "The event of pressing/releasing the joystick #5 key")
   LANGULUS_DEFINE_KEY(Joystick6,
      "The event of pressing/releasing the joystick #6 key")
   LANGULUS_DEFINE_KEY(Joystick7,
      "The event of pressing/releasing the joystick #7 key")
   LANGULUS_DEFINE_KEY(Joystick8,
      "The event of pressing/releasing the joystick #8 key")
   LANGULUS_DEFINE_KEY(Joystick9,
      "The event of pressing/releasing the joystick #9 key")
   LANGULUS_DEFINE_KEY(Joystick10,
      "The event of pressing/releasing the joystick #10 key")
   LANGULUS_DEFINE_KEY(Joystick11,
      "The event of pressing/releasing the joystick #11 key")
   LANGULUS_DEFINE_KEY(Joystick12,
      "The event of pressing/releasing the joystick #12 key")
   LANGULUS_DEFINE_KEY(Joystick13,
      "The event of pressing/releasing the joystick #13 key")
   LANGULUS_DEFINE_KEY(Joystick14,
      "The event of pressing/releasing the joystick #14 key")
   LANGULUS_DEFINE_KEY(Joystick15,
      "The event of pressing/releasing the joystick #15 key")
   LANGULUS_DEFINE_KEY(Joystick16,
      "The event of pressing/releasing the joystick #16 key")

} // namespace Langulus

#include "Event.inl"